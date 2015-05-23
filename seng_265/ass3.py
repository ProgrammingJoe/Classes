#!/opt/bin/python3
#Author: Joe Czepil, V00774878

import csv
import sys
from operator import itemgetter
import re

flo = re.compile('\d+(\.\d+)?')

def load(testcsv):
   try:
      thefile = open(testcsv)
      csvreader = csv.reader(thefile, delimiter=',')
      data = []
      for row in csvreader:
         data.append(row)
      return(data)
   except:
      print ("Error obtaining file: ", testcsv)
      reason = sys.exc_info()
      print ("Reason : ", reason[1])

def evalsum(data, colnum):
   rows = 0
   colsum = 0
   for row in data:
      rows += 1
      for col in row:
         colsum += 1
   cols = int((colsum)/(rows))
   if colnum >= cols:
      print ("The file only has %d columns, try again" %cols)
      return
   summ = []
   for rows in data:
      firstval = rows[colnum]
      if firstval.isdigit():
         summ.append(int(rows[1]))
      elif flo.match(firstval):
         summ.append(float(rows[colnum]))
      else: 
         print("Strings cannot be summed")
         return
   print (sum(summ))

def evalavg(data, colnum):
   rows = 0
   colsum = 0
   for row in data:
      rows += 1
      for col in row:
         colsum += 1
   cols = int((colsum)/(rows))
   if colnum >= cols:
      print ("The file only has %d columns, try again" %cols)
      return
   avg = []
   count = 1
   for rows in data:
      firstval = rows[colnum]
      if firstval.isdigit():
         avg.append(int(rows[1]))
         count += 1
      elif flo.match(firstval):
         avg.append(float(rows[colnum]))
         count += 1
      else: 
         print("Strings cannot be averaged")
         return
   avgg = (sum(avg))/count
   print (avgg)

def findrow(data, colnum, comp):
   rows = 0
   colsum = 0
   for row in data:
      rows += 1
      for col in row:
         colsum += 1
   cols = int((colsum)/(rows))
   if colnum > cols:
      print ("The file only has %d columns, try again" %cols)
      return
   count = 1
   for rows in data:
      if rows[colnum] == comp:
         print ("String found in row: ", count)
         return
      else:
         count += 1
   print ("The string was not found")   

def help():
   print ("This isn't very helpful :3")

def merge(data, newfile):
   try:
      thefile2 = open(newfile)
      csvreader2 = csv.reader(thefile2, delimiter=',')
      for row in csvreader2:
         data.append(row)
      return(data)
   except:
      print ("Error obtaining file: ", testcsv)
      reason = sys.exc_info()
      print ("Reason : ", reason[1])

def printrow(data, start, end):
   if start < 0 or end < 0:
      print ("Negative numbers are not accepted, try again")
      return
   if start > end:
      print("The first value has to be less than the second value")
      return
   if end > len(data):
      for rows in range(start, len(data)):
         print (data[rows])
   else:
      for rows in range(start, end):
         print (data[rows])

def save(data, name):
   f = open(name, 'w')
   for li in data:
      for elem in range(0, 4):
         f.write(li[elem])
         if elem != 3:
            f.write(',')
      f.write('\n')
   f.close()

def deleterow(data, row):
   if int(float(row)) < 0:
      print("Negative values are not accepted, try again")
      return
   if int(float(row)) > len(data):
      print("That row does not exist")
      return
   del data[(int(float(row))-1)]    

def sort(data, colnum):
   rows = 0
   colsum = 0
   for row in data:
      rows += 1
      for col in row:
         colsum += 1
   cols = int((colsum)/(rows))
   if colnum >= cols:
      print ("The file only has %d columns, try again" %cols)
      return
   for rows in data:
      data = sorted(data, key=itemgetter(colnum))
   print(data)
   return(data)
   
def sortnumeric(data, colnum):
   rows = 0
   colsum = 0
   for row in data:
      rows += 1
      for col in row:
         colsum += 1
   cols = int((colsum)/(rows))
   if colnum >= cols:
      print ("The file only has %d columns, try again" %cols)
      return
   for rows in data:
      data = sorted(data, key=itemgetter(colnum))
   print(data)
   return(data)

def stats(data):
   rows = 0
   colsum = 0
   for row in data:
      rows += 1
      for col in row:
         colsum += 1
   cols = int((colsum)/(rows))
   print("The number of rows is: ", rows)
   print("The number of columns is: ", cols)        

def main():
    print ('Please enter a command or type exit to exit: ')
    inp = input().split()
    while(inp[0] != 'exit'):
       
       if inp[0] == 'load':
          if len(inp) > 2:
             print ("Too many inputs were given, try again:")
             inp = input().split()
          else:
             testcsv = inp[1]
             data = load(testcsv)
             print (data)
             print ("You can type another command here! => ")
             inp = input().split()

       if inp[0] == 'deleterow':
          if len(inp) > 2:
             print ("Too many inputs were given, try again:")
             inp = input().split()
          else:
             deleterow(data, inp[1])
             print ("You can type another command here! => ")
             inp = input().split()

       elif inp[0] == 'evalavg':
          if len(inp) > 2:
             print ("Too many inputs were given, try again:")
             inp = input().split()
          elif inp[1].isdigit():
             colnum = int(inp[1])
             evalavg(data, colnum)
             print ("You can type another command here! => ")
             inp = input().split()
          else:
             upper = inp[1].upper()
             colnum = (ord(upper) - ord('A'))
             evalavg(data, colnum)
             print ("You can type another command here! => ")
             inp = input().split()

       elif inp[0] == 'evalsum':
          if len(inp) > 2:
             print ("Too many inputs were given, try again:")
             inp = input().split()
          elif inp[1].isdigit():
             colnum = int(inp[1])
             evalsum(data, colnum)
             print ("You can type another command here! => ")
             inp = input().split()
          else:
             upper = inp[1].upper()
             colnum = (ord(upper) - ord('A'))
             evalsum(data, colnum)
             print ("You can type another command here! => ")
             inp = input().split()

       elif inp[0] == 'findrow':
          if len(inp) > 3:
             print ("Too many inputs were given, try again:")
             inp = input().split()
          elif inp[1].isdigit():
             colnum = int(inp[1])
             findrow(data, colnum, inp[2])
             print ("You can type another command here! => ")
             inp = input().split()
          else:
             upper = inp[1].upper()
             colnum = (ord(upper) - ord('A'))
             findrow(data, colnum, inp[2])
             print ("You can type another command here! => ")
             inp = input().split()

       elif inp[0] == 'merge':
          if len(inp) > 2:
             print ("Too many inputs were given, try again:")
             inp = input().split()
          else:
             merge(data, inp[1])
             print ("The files have been merged. You can type another command here! => ")
             inp = input().split()   
                
       elif inp[0] == 'printrow':
          if len(inp) > 3:
             print ("Too many inputs were given")
             inp = input().split()
          elif len(inp) <= 2:
             printrow(data, 0, int(float(inp[1])))
             print ("You can type another command here! => ")
             inp = input().split()
          else:
             printrow(data, int(float(inp[1])), int(float((inp[2]))) )
             print ("You can type another command here! => ")
             inp = input().split()
       
       elif inp[0] == 'save':
          if len(inp) > 2:
             print ("Too many inputs were given, try again:")
             inp = input().split()
          elif len(inp) == 1:
             print ("Save requires a new file name as an argument, try again please:")
             inp = input().split()
          else:
             name = inp[1]
             save(data, name)
             print("The file has been saved. You can type another command here! => ")
             inp = input().split()
       
       elif inp[0] == 'sort':
          if len(inp) > 2:
             print ("Too many inputs were given, try again:")
             inp = input().split()
          elif inp[1].isdigit():
             colnum = int(inp[1])
             data = sort(data, colnum)
             print ("You can type another command here! => ")
             inp = input().split()
          else:
             upper = inp[1].upper()
             colnum = (ord(upper) - ord('A'))
             data = sort(data, colnum)
             print ("You can type another command here! => ")
             inp = input().split()
       
       elif inp[0] == 'sortnumeric':
          if len(inp) > 2:
             print ("Too many inputs were given, try again:")
             inp = input().split()
          elif inp[1].isdigit():
             colnum = int(inp[1])
             data = sortnumeric(data, colnum)
             print ("You can type another command here! => ")
             inp = input().split()
          else:
             upper = inp[1].upper()
             colnum = (ord(upper) - ord('A'))
             data = sortnumeric(data, colnum)
             print ("You can type another command here! => ")
             inp = input().split()

       elif inp[0] == 'stats':
          if len(inp) > 1:
             print ("No inputs are needed, try again:")
             inp = input().split()
          else:
             stats(data)
             print ("You can type another command here! => ")
             inp = input().split()
 
       elif inp[0] == 'help':
          print ("////////////////////////////////////////////////////////////////")
          print ("Step 1: Type 'load filename' where filename is an csv file you want loaded")
          print ("There are a number of functions you can do after step 1:")
          print ("")
          print ("Typing 'deleterow #' will delete row #")
          print ("Typing 'evalavg #/letter' or 'evalsum #/letter' will evaluate the sum or average of that column")
          print ("Typing 'findrow #/letter text' will find the row in column #/letter with text 'text'")
          print ("Typing 'merge filename' will merge your current csv file with another csv file")
          print ("Typing 'printrow #1 #2' will printrows #1 through #2 or if no #2 is given it will print rows 0 through #1")
          print ("Typing 'save filename' will save the file as a new filename.")
          print ("Typing 'sort #/letter' or 'sortnumeric #/letter' will sort the rows of the spreadsheet by the values in that column")
          print ("Typing 'stats' with no arguments will print the row numbers and column numbers of the file")
          print ("")
          print ("No commands in this program will save the file besides 'save'. It would be unfriendly to the user to save a file after they do something without their command")
          print ("////////////////////////////////////////////////////////////////")
          inp = input().split()

       else:
          print ("No proper command was inputted, type 'help' for assistance")
          inp = input().split()    

if __name__ == "__main__":
    main()
