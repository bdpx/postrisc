
# openssl dhparam -out dh.pem 4096

#openssl req -x509 -out localhost.crt -keyout localhost.key \
#  -newkey rsa:2048 -nodes -sha256 \
#  -subj '/CN=localhost' -extensions EXT -config <( \
#   printf "[dn]\nCN=localhost\n[req]\ndistinguished_name = dn\n[EXT]\nsubjectAltName=DNS:localhost\nkeyUsage=digitalSignature\nextendedKeyUsage=serverAuth")

#openssl genrsa -out ssl.key 2048
#openssl req -new -config ssl.conf -key ssl.key -out ssl.csr
#openssl x509 -req -sha256 -days 10000 -CAcreateserial -CAkey root.key -CA root.crt -in ssl.csr -out ssl.crt


openssl req -x509 -newkey rsa:4096 -sha256 -days 10000 -nodes -keyout key.pem -out cert.pem -config ssl.conf

#check created certificate
openssl x509 -in cert.pem -noout -text > cert_txt.txt