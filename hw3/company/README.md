# Company Tracker Program

**Assignment Description:** 

  Startups these days are merging so fast, it's hard to keep track of who is in what company. Company A merges with Company B, and Company C merges with Company D, and then the two merged companies merge, and suddenly, employees from companies A and C find themselves being colleagues. This is getting sufficiently difficult to follow that we will have you write a Startup Tracker.

  Here is how this will work. You have n students. Initially, each starts a startup company by himself/herself. Then, companies may merge. When you have a merge command, you will be given the numbers of two representative students, one from each company. Then, for each of those two students, you find the "biggest" company they are in, i.e., companies that are not subcompanies of any bigger company; let's call them A and B. Those two companies A and B are then merged into a new company; let's call it C. C will become the parent company of A and B.

  Sometimes, companies also split up again. When we call split, we will again give you a representative student. Then, you find the biggest company that the student is in - let's call it A. As a result of the split, A should be completely removed, and the two companies that were at some point in the past merged to become A will now be individual companies without a parent again. (If the student is only in their own 1-person company, split does nothing.)

  You will build a data structure that allows you to process a sequence of merges and splits, and answer queries about whether two students are in the same company.
