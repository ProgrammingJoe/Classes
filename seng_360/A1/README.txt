Joe Czepil
V00774878
September 16th, 2015

FILES CONTAINED:
-seng360.py
-license.txt
-README.txt

HOW TO RUN THE PROGRAM:
1) Open license.txt and enter a date with the format <day/month/year> in number format.
2) Run "python seng360.py".

If the date in license.txt is past december 31st 2015 then the license has expired. Otherwise the license will always be successful decrypted because the "reciever" is given the correct dig sig and public decryption key.

WHAT seng360.py DOES:
1) Checks license.txt for it's expirationd date. 
2) Hashes license.txt for to create a message digest.
3) Hashes and creates a public key for the digital signature.
4) Uses the public key to decrypt the digital signature.
5) Compares the two message digests, if equal (which they will be) a secret message is displayed to the screen.
