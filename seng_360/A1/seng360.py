
import os
import sys
from Crypto.Hash import MD5
from Crypto.Random import random
from Crypto.PublicKey import DSA
from Crypto.Hash import SHA

def main():
	md = hashmessage()
	ver = digsig(md)
	checkdigsig(ver)

"""
Check the date of the license to make sure it is still valid.
Licences with a date before the year 2016 are valid.
"""
def checkdate(data):
	day, month, year = data.split('/')
	if(year <= '2015'):
		if(month <= '12'):
			if(day <= '31'):
				return
	print('License has expired.')
	sys.exit()

"""
Hash the message to return a message digest.
"""
def hashmessage():
	try:
		with open ("licenseseng.txt", "r") as myfile:
			data=myfile.read().replace('\n', '')
	except:
		print "License not found. Terminating program..."
		sys.exit()

	checkdate(data)

	md = MD5.new()
	md.update(data)
	md = md.hexdigest()
	return md

"""
Given the message create a digital signature and public key.
Return the public key.
"""
def digsig(md):
	key = DSA.generate(1024)
	md = SHA.new(md).digest()
	k = random.StrongRandom().randint(1,key.q-1)
	sig = key.sign(md,k)
	hacks = (md, sig, key)
	return hacks

"""
Given the public key, digital signature, and message digest verify the digital signature.
"""
def checkdigsig(ver):
	if ver[2].verify(ver[0],ver[1]):
		print "Super secret message."
	else:
		print "License Invalid"


if __name__ == "__main__":
    main()
