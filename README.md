# COP4520assignment1
written in c++

To run: in a linux terminal such as ubuntu, navigate to the directory containing the primes.cpp file. 
Then run the command "g++ -fpermissive prime.cpp  -lpthread". Once that is complete run the command "./a.out". 
The primes.txt file containing the output should be created in the same directory as parallel1.cpp.

THE TIME OUTPUT IS IN SECONDS. For example, if the first number in primes.txt is "8", that means it took 8 seconds to run.

I have done prime number tests before, by simply iterating through every number and testing it for primes, but that would be incredibly slow with this high a range even with multiple threads. So I researched better prime tests and found this page: https://en.wikipedia.org/wiki/Primality_test, which helped me come up with my final 6k+-1 brute force approach. I also tried a seive algorithm initially but ultimately abandoned it because of code complexity; a seive may be faster, but my approach still finishes in a reasonable time. On my hardware my final approach takes 7-8 seconds to complete. I believe my final prime tally and total sum is correct because my result matched my peers' results. 

