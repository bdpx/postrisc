#include <boost/test/unit_test.hpp>

#include "util/common.hpp"
#include "emulator/aes.hpp"

/*
 test the various instructions, using the examples provided
      in  "White Paper Intel Advanced Encryption Standard AES
          instruction set" January 2010 (26/1/2010)
          Rev. 3.0
          by Shay Gueron
   AESKEYGENASSIST(1,
                   "3c4fcf098815f7aba6d2ae2816157e2b",
                   "01eb848beb848a013424b5e524b5e434");
   AESENC("7b5b54657374566563746f725d53475d",
          "48692853686179295b477565726f6e5d",
          "a8311c2f9fdba3c58b104b58ded7e595");
   AESENCLAST("7b5b54657374566563746f725d53475d",
              "48692853686179295b477565726f6e5d",
              "c7fb881e938c5964177ec42553fdc611");
   AESDEC("7b5b54657374566563746f725d53475d",
          "48692853686179295b477565726f6e5d",
          "138ac342faea2787b58eb95eb730392a");
   AESDECLAST("7b5b54657374566563746f725d53475d",
              "48692853686179295b477565726f6e5d",
              "c5a391ef6b317f95d410637b72a593d0");

   the AESIMC example given in the Intel White paper seems wrong.
   The below fails both under Valgrind and natively.
      AESIMC("48692853686179295b477565726f6e5d",
             "627a6f6644b109c82b18330a81c3b3e5");
      So we use the example given for the InvMixColums 
      transformation.
   AESIMC("8dcab9dc035006bc8f57161e00cafd8d",
          "d635a667928b5eaeeec9cc3bc55f5777");
*/

namespace postrisc {

BOOST_AUTO_TEST_SUITE(aes)

static u8 fromhex(char x) {
   if      (x >= '0' && x <= '9') { return(x - '0'); }
   else if (x >= 'A' && x <= 'F') { return(x - 'A' + 10); }
   else if (x >= 'a' && x <= 'f') { return(x - 'a' + 10); }
   else assert(0);
   return 0;
}

static void expand(GeneralRegister& dst, char const summary[32])
{
   for (size_t i = 0; i < 16; i++) {
      char const x = summary[31-2*i];
      char const y = summary[31-2*i-1];
      u8 xx = fromhex (x);
      u8 yy = fromhex (y);
      assert(xx < 16);
      assert(yy < 16);
      xx = (yy << 4) | xx;
      dst.vector<u8>(i) = xx;
   }
}

static void reverse(GeneralRegister& dst)
{
   for (size_t i = 0, j = 15; i < j; i++, j--) {
      std::swap(dst.vector<u8>(i), dst.vector<u8>(j));
   }
}

BOOST_AUTO_TEST_CASE(test_aeskeygenassist)
{
    static const struct {
        u8   rcon;
        char key[33];
        char result[33];
    } tests[] = {
        { 1, "3c4fcf098815f7aba6d2ae2816157e2b", "34e4b524e5b52434018a84eb8b84eb01" },
        { 8, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", "eaeaeaeae2eaeaeaeaeaeaeae2eaeaea" },
        { 8, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "acacacaca4acacacacacacaca4acacac" },
        { 2, "3243f6a8885a308d313198a2e0370734", "3a46c7c744c7c73ac2421a23401a23c2" },
        { 2, "193de3bea0f4e22b9ac68d2ae9f84808", "e55db4b85fb4b8e5ae1127d41327d4ae" },
        { 1, "d4bf5d30e0b452aeb84111f11e2798e5", "a182836c83836ca1044c08484d084804" },
        { 1, "a0fafe1788542cb123a339392a6c7605", "12120a26130a2612f0bb2de0ba2de0f0" },
        { 8, "49ded28945db96f17f39871a7702533b", "a21712d21f12d2a2a7b51d3bbd1d3ba7" },
        { 8, "584dcaf11b4b5aacdbe7caa81b6bb0e5", "c27494b97c94b9c2a174e36a7ce36aa1" },
        { 2, "aa8f5f0361dde3ef82d24ad26832469a", "b5d6b513d4b513b57bcf73accd73ac7b" },
        { 2, "acc1d6b8efb55a7b1323cfdf457311b5", "9e8a267d88267d9e6cf67891f478916c" },
        { 1, "e9317db5cb322c723d2e895faf090794", "cfa73127a63127cfd5ffc71efec71ed5" },
    };

    for (auto const& test : tests) {
        GeneralRegister key;
        expand(key, test.key);
        GeneralRegister result;
        expand(result, test.result);
        GeneralRegister ret = aes::keygen_assist(key, test.rcon);
        reverse(ret);
        BOOST_TEST(ret == result,
            fmt::endl
            << "key = " << key << fmt::endl
            << "rcon = " << fmt::hex(test.rcon) << fmt::endl
            << "rer  = " << ret << fmt::endl
            << "result = " << result
        );
    }
}

BOOST_AUTO_TEST_CASE(test_aes)
{
    static const struct {
        char src1[33];
        char src2[33];
        char aesenc_result[33];
        char aesenclast_result[33];
        char aesdec_result[33];
        char aesdeclast_result[33];
        char aesimc_result[33];
    } tests[] = {
// aesimc 8dcab9dc035006bc8f57161e00cafd8d result 77575fc53bccc9eeae5e8b9267a635d6
        { "7b5b54657374566563746f725d53475d", "48692853686179295b477565726f6e5d",
          "95e5d7de584b108bc5a3db9f2f1c31a8", "11c6fd5325c47e1764598c931e88fbc7",
          "2a3930b75eb98eb58727eafa42c38a13", "d093a5727b6310d4957f316bef91a3c5",
          "6d0b9ac9808d60a83d4928fb88132ca7" },
        { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
          "17171717171717171717171717171717", "17171717171717171717171717171717",
          "d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9", "d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9",
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" },
        { "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
          "40404040404040404040404040404040", "40404040404040404040404040404040",
          "54545454545454545454545454545454", "54545454545454545454545454545454",
          "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" },
        { "3243f6a8885a308d313198a2e0370734", "2b7e151628aed2a6abf7158809cf4f3c",
          "5b21939f0be8c5228779cc82ded0bbe7", "2409712ab211ed4afb9034efd4d0b9ef",
          "1a25b0ac361c8eb4fddc79ef4827a878", "14998927922d933c12301c89791d508b",
          "db530b675db2ba6fa708ae6ed9f66c6c" },
        { "193de3bea0f4e22b9ac68d2ae9f84808", "d42711aee0bf98f1b8b45de51e415230",
          "dcd62e1fbee4a50fc2300ed34b3c9300", "000ffeca00c593a60089fe5800439334",
          "ceb3447b03c44db310640f9d67b7e938", "8f1ffb2970893ffffa2c5e6ef42ae03f",
          "8a6ed86d802581dfeac414a7a21dd315" },
        { "d4bf5d30e0b452aeb84111f11e2798e5", "046681e5e0cb199a48f8d37a2806264c",
          "bd2484c75918df5c3d46548f45760114", "95a48b60dbd3f03a7e55078ce1c7e5e5",
          "cc0387f79017ecef380418f56fefbe69", "66abc0b251310ce824faf6f9edc99eed",
          "6881228f0df28f69bae4eb1dd6a65523" },
        { "a0fafe1788542cb123a339392a6c7605", "a49c7ff2689f352b6b5bea43026a5049",
          "699f68563c8856d00046268c8f25170e", "22424ae2519b768ee38ecf4e02479660",
          "16410fdca2b0401cd1e0745e2318f956", "7f5c973018e54ffc7d6e272f753ded31",
          "daafd9998b920a93560531231bc98bea" },
        { "49ded28945db96f17f39871a7702533b", "49db873b453953897f02d2f177de961a",
          "7eb10f92ed990efeb1c8aec03eb90798", "f881674c53421f8a28e64e979c6ac927",
          "c2ea4f805b7fa23f53f6bcb0ccb0a26e", "53e9411cb2829e17a2b953e1c9b2804b",
          "fa94f98aee746021e71d24278e665a7e" },
        { "584dcaf11b4b5aacdbe7caa81b6bb0e5", "f2c295f27a96b9435935807a7359f67f",
          "9363371070b47e1c20f85db0a2e95d3c", "a682ea19b83ed6f6d2cde9c35372565d",
          "98438a9b55d66fc8a408075c6411e65d", "55e695ec157c501de9a99324d9d372b6",
          "57919370d34c20e10b86bb9004e4e42a" },
        { "aa8f5f0361dde3ef82d24ad26832469a", "ac73cf7befc111df13b5d6b545235ab8",
          "dfa13bc2c191383f003da25f392b090b", "008ce2e900c7c65600dee2fc0095c643",
          "39388a643582a19fa61b9f3c8ba80951", "8fdeea54ca4ec6cbbe4d608dae820c5b",
          "75cccaf52bbea0fd78e6311f21815f86" },
        { "acc1d6b8efb55a7b1323cfdf457311b5", "75ec0993200b633353c0cf7cbb25d0dc",
          "acff63b994d0a2f082425ec02fcf34c0", "095af02ae271b83d1295845dff8bcaaa",
          "f76e85460429f14c12e254a6e6c52680", "0e9af739932c1d32303c848a094fde1d",
          "bdf0449b196aa5f6a2f39ab08010ff6c" },
        { "e9317db5cb322c723d2e895faf090794", "d014f9a8c9ee2589e13f0cc8b6630ca6",
          "2d008268600a9fef35a43ab963f1477e", "84ab40a8077df898c9daefee7d3c25cf",
          "c542e1c7784ea84f7e85c9fe3be30603", "411fc23d4c3411b897d7ae227abbd7cb",
          "6d0b9ac9808d60a83d4928fb88132ca7" },
    };

    for (auto const& test : tests) {
        GeneralRegister src1;  expand(src1, test.src1);
        GeneralRegister src2;  expand(src2, test.src2);

        GeneralRegister aesenc_result;
        expand(aesenc_result, test.aesenc_result);
        GeneralRegister ret = aes::encrypt(src1, src2);
        reverse(ret);
        BOOST_TEST(ret == aesenc_result,
            fmt::endl
            << "src1 = " << src1 << fmt::endl
            << "src2 = " << src2 << fmt::endl
            << "ret  = " << ret << fmt::endl
            << "res  = " << aesenc_result
        );

        GeneralRegister aesenclast_result;
        expand(aesenclast_result, test.aesenclast_result);
        ret = aes::encrypt_last(src1, src2);
        reverse(ret);
        BOOST_TEST(ret == aesenclast_result,
            fmt::endl
            << "src1 = " << src1 << fmt::endl
            << "src2 = " << src2 << fmt::endl
            << "ret  = " << ret << fmt::endl
            << "res  = " << aesenc_result
        );

        GeneralRegister aesdec_result;
        expand(aesdec_result, test.aesdec_result);
        ret = aes::decrypt(src1, src2);
        reverse(ret);
        BOOST_TEST(ret == aesdec_result,
            fmt::endl
            << "src1 = " << src1 << fmt::endl
            << "src2 = " << src2 << fmt::endl
            << "ret  = " << ret << fmt::endl
            << "res  = " << aesenc_result
        );

        GeneralRegister aesdeclast_result;
        expand(aesdeclast_result, test.aesdeclast_result);
        ret = aes::decrypt_last(src1, src2);
        reverse(ret);
        BOOST_TEST(ret == aesdeclast_result,
            fmt::endl
            << "src1 = " << src1 << fmt::endl
            << "src2 = " << src2 << fmt::endl
            << "ret  = " << ret << fmt::endl
            << "res  = " << aesenc_result
        );
}
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace postrisc
