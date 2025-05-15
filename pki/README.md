### CA files:
- ca.key (Private key)
- ca.crt (Self-signed cert)

### Server files:
- private.key (Server private key)
- public.key (Optional public key)
- server.csr (CSR)
- ca.cert (Signed server certificate)



# generate CSR:
```
openssl req -new -key private.key -out server.csr -subj "/C=BE/ST=Wallois/L=Mons/O=UMons/OU=rpi/CN=raspberrypiG3.local"
```

# sign the CSR:

```
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out ca.cert -days 825 -sha256 -extfile openssl_ext.cnf -extensions v3_req
```

# generate DER:
```
openssl x509 -in ca.cert -outform DER -out ca.cert.DER
xxd -i ca.cert.DER
```
