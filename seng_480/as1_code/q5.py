from sklearn.feature_extraction.text import CountVectorizer
import numpy as np


def main():
    with open('t8.shakespeare.txt', 'r') as f:
        shakespeare =[f.read().replace('\n', '')]
    vectorizer = CountVectorizer(lowercase=True, ngram_range=(1, 2), stop_words=None, max_df=1.0, min_df=1, max_features=None)
    X_test = vectorizer.fit_transform(shakespeare).toarray()
    sorted_counts = np.argsort(X_test[0])
    top15 = sorted_counts[-16:]
    words = vectorizer.get_feature_names()
    for topword in top15:
        print(str(words[topword] + "||" + str(X_test[0][topword])))


if __name__ == '__main__':
   main()

# Copyright for t8.shakespeare.txt
#
#   Since unlike many other Project Gutenberg-tm etexts, this etext
# is copyright protected, and since the materials and methods you
# use will effect the Project's reputation, your right to copy and
# distribute it is limited by the copyright and other laws, and by
# the conditions of this "Small Print!" statement.
#
# 1.  LICENSE
#
#   A) YOU MAY (AND ARE ENCOURAGED) TO DISTRIBUTE ELECTRONIC AND
# MACHINE READABLE COPIES OF THIS ETEXT, SO LONG AS SUCH COPIES
# (1) ARE FOR YOUR OR OTHERS PERSONAL USE ONLY, AND (2) ARE NOT
# DISTRIBUTED OR USED COMMERCIALLY.  PROHIBITED COMMERCIAL
# DISTRIBUTION INCLUDES BY ANY SERVICE THAT CHARGES FOR DOWNLOAD
# TIME OR FOR MEMBERSHIP.
#
#   B) This license is subject to the conditions that you honor
# the refund and replacement provisions of this "small print!"
# statement; and that you distribute exact copies of this etext,
# including this Small Print statement.  Such copies can be
# compressed or any proprietary form (including any form resulting
# from word processing or hypertext software), so long as
# *EITHER*:
#
#     (1) The etext, when displayed, is clearly readable, and does
#   *not* contain characters other than those intended by the
#   author of the work, although tilde (~), asterisk (*) and
#   underline (_) characters may be used to convey punctuation
#   intended by the author, and additional characters may be used
#   to indicate hypertext links; OR
#
#     (2) The etext is readily convertible by the reader at no
#   expense into plain ASCII, EBCDIC or equivalent form by the
#   program that displays the etext (as is the case, for instance,
#   with most word processors); OR
#
#     (3) You provide or agree to provide on request at no
#   additional cost, fee or expense, a copy of the etext in plain
#   ASCII.
#
# 2.  LIMITED WARRANTY; DISCLAIMER OF DAMAGES
#
# This etext may contain a "Defect" in the form of incomplete,
# inaccurate or corrupt data, transcription errors, a copyright or
# other infringement, a defective or damaged disk, computer virus,
# or codes that damage or cannot be read by your equipment.  But
# for the "Right of Replacement or Refund" described below, the
# Project (and any other party you may receive this etext from as
# a PROJECT GUTENBERG-tm etext) disclaims all liability to you for
# damages, costs and expenses, including legal fees, and YOU HAVE
# NO REMEDIES FOR NEGLIGENCE OR UNDER STRICT LIABILITY, OR FOR
# BREACH OF WARRANTY OR CONTRACT, INCLUDING BUT NOT LIMITED TO
# INDIRECT, CONSEQUENTIAL, PUNITIVE OR INCIDENTAL DAMAGES, EVEN IF
# YOU GIVE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
#
# If you discover a Defect in this etext within 90 days of receiv-
# ing it, you can receive a refund of the money (if any) you paid
# for it by sending an explanatory note within that time to the
# person you received it from.  If you received it on a physical
# medium, you must return it with your note, and such person may
# choose to alternatively give you a replacement copy.  If you
# received it electronically, such person may choose to
# alternatively give you a second opportunity to receive it
# electronically.
#
# THIS ETEXT IS OTHERWISE PROVIDED TO YOU "AS-IS".  NO OTHER
# WARRANTIES OF ANY KIND, EXPRESS OR IMPLIED, ARE MADE TO YOU AS
# TO THE ETEXT OR ANY MEDIUM IT MAY BE ON, INCLUDING BUT NOT
# LIMITED TO WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A
# PARTICULAR PURPOSE.  Some states do not allow disclaimers of
# implied warranties or the exclusion or limitation of consequen-
# tial damages, so the above disclaimers and exclusions may not
# apply to you, and you may have other legal rights.
#
# 3.  INDEMNITY: You will indemnify and hold the Project, its
# directors, officers, members and agents harmless from all lia-
# bility, cost and expense, including legal fees, that arise
# directly or indirectly from any of the following that you do or
# cause: [A] distribution of this etext, [B] alteration,
# modification, or addition to the etext, or [C] any Defect.
#
# 4.  WHAT IF YOU *WANT* TO SEND MONEY EVEN IF YOU DON'T HAVE TO?
# Project Gutenberg is dedicated to increasing the number of
# public domain and licensed works that can be freely distributed
# in machine readable form.  The Project gratefully accepts
# contributions in money, time, scanning machines, OCR software,
# public domain etexts, royalty free copyright licenses, and
# whatever else you can think of.  Money should be paid to "Pro-
# ject Gutenberg Association / Illinois Benedictine College".
#
# WRITE TO US! We can be reached at:
#      Internet: hart@vmd.cso.uiuc.edu
#        Bitnet: hart@uiucvmd
#    CompuServe: >internet:hart@.vmd.cso.uiuc.edu
#       Attmail: internet!vmd.cso.uiuc.edu!Hart
#         Mail:  Prof. Michael Hart
#                P.O. Box 2782
#                Champaign, IL 61825
#
# This "Small Print!" by Charles B. Kramer, Attorney
# Internet (72600.2026@compuserve.com); TEL: (212-254-5093)
# ****   SMALL PRINT! FOR __ COMPLETE SHAKESPEARE ****
# ["Small Print" V.12.08.93]
#
# <<THIS ELECTRONIC VERSION OF THE COMPLETE WORKS OF WILLIAM
# SHAKESPEARE IS COPYRIGHT 1990-1993 BY WORLD LIBRARY, INC., AND IS
# PROVIDED BY PROJECT GUTENBERG ETEXT OF ILLINOIS BENEDICTINE COLLEGE
# WITH PERMISSION.  ELECTRONIC AND MACHINE READABLE COPIES MAY BE
# DISTRIBUTED SO LONG AS SUCH COPIES (1) ARE FOR YOUR OR OTHERS
# PERSONAL USE ONLY, AND (2) ARE NOT DISTRIBUTED OR USED
# COMMERCIALLY.  PROHIBITED COMMERCIAL DISTRIBUTION INCLUDES BY ANY
# SERVICE THAT CHARGES FOR DOWNLOAD TIME OR FOR MEMBERSHIP.>>
#
#
# 1609
#
# THE SONNETS
#
# by William Shakespeare