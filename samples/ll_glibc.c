/* This code implements ll in C code using the c library */
/* Note!  This only will work on x86.  It has endianess  */
/*        dependencies, and makes assumptions about the  */
/*        layout of the /proc/cpuinfo file!              */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//#include <sys/utsname.h>
struct utsname {
    char sysname[256/*_UTSNAME_SYSNAME_LENGTH*/];
    char nodename[256/*_UTSNAME_NODENAME_LENGTH*/];
    char release[256/*_UTSNAME_RELEASE_LENGTH*/];
    char version[256/*_UTSNAME_VERSION_LENGTH*/];
    char machine[256/*_UTSNAME_MACHINE_LENGTH*/];
    char domainname[256/*_UTSNAME_DOMAIN_LENGTH*/];
};
extern int uname(struct utsname *__name);


//#include <sys/sysinfo.h>

struct sysinfo {
               long uptime;             /* Seconds since boot */
               unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
               unsigned long totalram;  /* Total usable main memory size */
               unsigned long freeram;   /* Available memory size */
               unsigned long sharedram; /* Amount of shared memory */
               unsigned long bufferram; /* Memory used by buffers */
               unsigned long totalswap; /* Total swap space size */
               unsigned long freeswap;  /* Swap space still available */
               unsigned short procs;    /* Number of current processes */
               unsigned long totalhigh; /* Total high memory size */
               unsigned long freehigh;  /* Available high memory size */
               unsigned int mem_unit;   /* Memory unit size in bytes */
               char _f[20-2*sizeof(long)-sizeof(int)];
                                        /* Padding to 64 bytes */
};
extern int sysinfo(struct sysinfo *info);

__attribute__((noinline)) int uname(struct utsname *__name)
{
  return 0;
}

__attribute__((noinline)) int sysinfo(struct sysinfo *info)
{
  return 0;
}

#define N 1024
#define F 64
#define THRESHOLD 2
#define P_BITS 10
#define POSITION_MASK 3
unsigned char logo[]={
	255, 27, 91, 48, 59, 49, 59, 51, 55,
	159, 59, 52, 55, 109, 35, 204, 247, 192, 7, 51,
	141, 48, 200, 27, 27, 91, 196, 7, 203, 31, 28, 12, 59,
	15, 52, 48, 109, 10, 192, 247, 1, 96, 26, 56, 44, 156,
	31, 27, 91, 51, 49, 109, 204, 4, 65, 172, 13, 36,
	2, 28, 16, 79, 13, 32, 16, 65, 147, 152, 131, 52, 28, 52, 204, 16,
	16, 12, 36, 111, 57, 236, 167, 28, 8, 51, 22, 20, 137, 85, 44, 96,
	0, 43, 97, 214, 113, 226, 200, 203, 8, 212, 9, 211, 16, 43, 89, 245, 209,
	0, 128, 17, 210, 24, 13, 40, 28, 20, 13, 44, 28, 28, 240, 74, 26, 91,
	0, 13, 80, 95, 101, 135, 101, 43, 85, 245, 205, 205, 40, 205, 20, 137, 65,
	0, 29, 135, 66, 75, 114, 83, 28, 120, 15, 98, 135, 109, 85, 88, 247, 193,
	0, 232, 43, 244, 151, 73, 120, 61, 176, 27, 95, 151, 176, 18, 43, 171, 202,
	16, 223, 22, 26, 245, 90, 245, 217, 63, 51, 27, 86, 146, 91, 176, 2,
	0, 12, 29, 211, 200, 172, 57, 23, 102, 50, 246, 110, 109, 236, 68, 96, 94,
	8, 175, 10, 166, 105, 20, 1, 48, 51, 11, 222, 31, 49, 15, 211, 188,
	0, 175, 79, 25, 86, 170, 69, 82, 219, 40, 82, 70, 127, 8, 83, 219, 35,
	0, 169, 85, 170, 53, 24, 33, 18, 104, 145, 42, 200, 34, 178, 104, 112, 45,
	0, 198, 80, 178, 121, 145, 74, 112, 49, 248, 81, 243, 40, 221, 23, 255, 23,
	8,2,54,3,36,229,66,10,
};

unsigned int r,shift_count,bit_set;
unsigned short out_count,pos;
unsigned char mask,out_byte;
char *logo_pointer,*out_pointer,*disk_pointer;
char out_buffer[16384],text_buf[(N+F)-1],disk_buffer[4096];

struct utsname uname_info;
struct sysinfo sysinfo_buff;
int fd,cpu_count,temp;

char ordinal[4][6]={"One","Two","Three","Four"};

   /* Write a string to stdout */
static void write_stdout(char *string) {
   
      /* stdout is fd 1 */
   write(1,string,strlen(string));
}

     /* find the 4 ascii bytes in pattern.  Then skip to : then skip spaces */
     /* then copy the string found to the end of out_pointer, stopping at   */
     /* the stop character or \n                                            */
static void find_string(unsigned int pattern,unsigned char stop,int skip_spaces) {

   unsigned char ch;

   if (!skip_spaces) {
	 
      disk_pointer=disk_buffer;
      out_pointer=out_buffer+strlen(out_buffer);

         /* Find pattern */
      while(1) {
         temp=*((int *)(disk_pointer));
         if (temp==pattern) break;
         if (temp==0) return;
         disk_pointer++;
      }
   
         /* skip to after a colon */
      while(*disk_pointer++!=':') ;
    
         /* skip to after any spaces */
      while(*disk_pointer++==' ') ;
      disk_pointer--;
   }
   
        /* copy from the disk buffer to the output buffer */
   do {
      ch=*out_pointer++=*disk_pointer++;
   } while((ch!=stop) && (ch!='\n'));
   
        /* Null terminate the result */
   *--out_pointer=0;
   
}

char ascii_buff[8];
char *ascii_pointer;

   /* convert an integer to an ascii string */
static char *num_to_ascii(unsigned int num) {

   unsigned int q,r,temp;

   temp=num;
   ascii_pointer=ascii_buff+7;
   
   /* do loop so we print "0" properly */
   do {      
      q=temp/10;
      r=temp%10;
      
      *--ascii_pointer=(unsigned char)(r+0x30);      /* convert to ASCII */
      temp=q;
   } while(temp);

   return ascii_pointer;
}

/* center and print a string */
static void center_and_print() {
   int length;
   
   length=strlen(out_buffer);
   if (length<80) {
      write_stdout("\033[");
      write_stdout(num_to_ascii((80-length)/2));
      write_stdout("C");
   }
   write_stdout(out_buffer);   
}

__attribute__((section(".text")))
int main(int argc, char **argv)
{
   /* do LZSS decryption */
   
   r=(N-F);
   logo_pointer=(char *)logo;
   out_pointer=out_buffer;
   
   while(logo_pointer<((char *)logo+sizeof(logo)) ) {
            
      shift_count=0;
      mask=(*logo_pointer++);

      while(shift_count<8) {
      
	 if (logo_pointer>=((char *)logo+sizeof(logo)) ) break;
	 
         bit_set=mask&1;
         mask>>=1;   
   
         if (bit_set) {
            out_byte=(*logo_pointer++);
            out_count=1;
         }
         else {
             /* Litle Endian assumption */   
            pos=* ((short *)logo_pointer);
            logo_pointer+=2;
	    out_count=(unsigned short)((pos>>P_BITS)+(THRESHOLD+1));
         }
   
	 while(out_count) {
	    if (!bit_set) {
               pos&=((POSITION_MASK<<8)+0xff);
               out_byte=text_buf[pos++];
	    }

            (*out_pointer++)=out_byte;
            text_buf[r++]=out_byte;
            r&=(N-1);
            out_count--;
	 }
         shift_count++;
      }
   }
   
   write_stdout(out_buffer);
      
   /* first line */
   uname(&uname_info);
   out_buffer[0]=0;
   strcat(out_buffer,uname_info.sysname);
   strcat(out_buffer," Version ");
   strcat(out_buffer,uname_info.release);
   strcat(out_buffer,", Compiled ");
   strcat(out_buffer,uname_info.version);
   strcat(out_buffer,"\n");
   
   center_and_print();
   
   /* middle line */
   fd=open("/proc/cpuinfo",0,0);
   read(fd,disk_buffer,4096);
   close(fd);
   out_buffer[0]=0;
   
   /* count CPUs */
   cpu_count=0;
   disk_pointer=disk_buffer;
   do {
      temp=*((int *)(disk_pointer));
      if (temp==('o'<<24)+('g'<<16)+('o'<<8)+'b') cpu_count++;
      disk_pointer++;
   } while (temp!=0);
   strcat(out_buffer,ordinal[cpu_count-1]);
   strcat(out_buffer," ");
   
   find_string(('z'<<24)+('H'<<16)+('M'<<8)+' ','.',0);
   strcat(out_buffer,"MHz ");
   find_string(('e'<<24)+('m'<<16)+('a'<<8)+'n',' ',0);
   strcat(out_buffer," ");
   out_pointer++;
   find_string(('e'<<24)+('m'<<16)+('a'<<8)+'n',' ',1);
   strcat(out_buffer," Processor");
   
   /* handle plural */
   if (cpu_count!=1) strcat(out_buffer,"s"); 
   strcat(out_buffer," ");
   sysinfo(&sysinfo_buff);
   strcat(out_buffer,num_to_ascii(sysinfo_buff.totalram>>20));
   strcat(out_buffer,"M RAM, ");
   find_string(('s'<<24)+('p'<<16)+('i'<<8)+'m','\n',0);
   strcat(out_buffer," Bogomips Total\n");
   center_and_print();
   
   /* last line */
   out_buffer[0]=0;
   strcat(out_buffer,uname_info.nodename);
   center_and_print();
   
   /* restore colors */
   out_buffer[0]=0;
   strcat(out_buffer,"\33[0m\n\n");
   write_stdout(out_buffer);
   
   return 0;
}
