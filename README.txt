# ME-C3100 Computer Graphics, Fall 2017
# Lehtinen / Aarnio, Kemppinen, Ollikainen
#
# Assignment 5: Ray Tracing

Student name: Anıl YARIŞ
Student number: 643454
Hours spent on requirements (approx.): 15-20 hours
Hours spent on extra credit (approx.): 1-2 hours

# First, a 10-second poll about this assignment period:

Did you go to exercise sessions?
No.
Did you work on the assignment using Aalto computers, your own computers, or both?
Only my own computer.
# Which parts of the assignment did you complete? Mark them 'done'.
# You can also mark non-completed parts as 'attempted' if you spent a fair amount of
# effort on them. If you do, explain the work you did in the problems/bugs section
# and leave your 'attempt' code in place (commented out if necessary) so we can see it.

R1 Generating rays & ambient lighting (  1p): done
R2 Visualizing depth		      (0.5p): done
R3 Perspective camera		      (0.5p): done
R4 Phong shading		      (  3p): done
R5 Plane intersection		      (0.5p): done
R6 Triangle intersection	      (  1p): done
R7 Shadows			      (  1p): done
R8 Mirror reflection		      (  1p): done
R9 Antialiasing			      (1.5p): done

# Did you do any extra credit work?

I implemented intersection of transforms.

# Are there any known problems/bugs remaining in your code?

As far as what I've been able to test there doesn't seem to be any.

# Did you collaborate with anyone in the class?

I sent some useful documents to 2 of my friends, such as this one: http://www.cs.cornell.edu/courses/cs4620/2011fa/lectures/06raytracingWeb.pdf

# Any other comments you'd like to share about the assignment or the course so far?

Note about assignment: I might have deviated from the methods of calculationg triangle intersection given in the slides as I couldn't get it 
to work properly. Instead I used the Möller–Trumbore algorithm. 

Feedback about assignment: I feel like based on the amount of work this assignment involved the total point for requirements should have been
greater than 10. It wouldn't strictly help if this assignment had higher percentage of contribution compared to previous assignments because I
didn't have time to implement some extras that I thought I could implement because of the time I spent in the requirements. One solution would 
be making some of the requirements extra as well, this would help to ensure that points that would be obtained per total hours spent would not 
be lower than the previous assignments. Another thing is that most of the time this assignment took me was spent on trying to fix the bugs caused 
by the lack/ambiguity of information in the lecture slides/the handout of the assignment about the thing needed to be implemented. Some examples 
to this issue: I didn't realize pixel indices started from the top left corner of the image rather than the lower left, I had a hard time trying 
to figure out how to incorporate the "up" and "horizontal" vectors to ray generation (that's why I had to look for the document in the URL above, 
after finding that document it took me 10 minutes to correctly implement R3 as opposed to the 3-4 hours I had previously spent on it), there was 
no information in the handout about the shade_back flag in the Phong shader so I had to rewrite my implementation to get it right, etc. If I didn't 
have to spend this much unnecessary time on the requirements alone I would have time to implement more extras and would get more points while having 
spent the same overall effort on the assignment.
