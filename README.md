# Sorter-Project
This is where we host our documentation and code for the Industrial Sorter Project.

# Minutes 11/29/20

We started our meeting looking at the data sheet for the cameras we will be using
    - voti.nl/docs/OV7670.pdf
We also discussed ways to reduce glare in the sorter cups
    - Using a cloth like felt to absorb light
    - Doing the filtering entirely with the image processing algorithm
We talked about ways of getting camera data from the microcontroller to a visible photo
    - Exporting data through UART into a text file, and parsing with some programming language


# Minutes 11/22/20

- We discussed the 3D printed figures from the TinkerCAD fiesta on Friday
    - The printing is going well, other than some more difficult organic shapes

- We discussed methods of holding hand sanitizer
    - Using some kind of sealant on the inside of a 3D printed enclosure
    - Designing parts to screw onto a standard bottle size
    - Using a bag on the inside of the enclosure
    - Partnering with the chemistry department for the solution
    - Solving the problem of viscosity to keep the dispensing speed reasonable
    - Nhat has a working prototype
    - Working on the software side of things

- We revisited the sorter machine design document
    - We accomplished a lot of what we set out to do
    - Some things we descoped, or determined weren't necessary

- We discussed Git and GitHub
    - Over winter break, we may do a workshop
    - Mathew has uploaded his code to GitHub

- We talked about cameras for the image processing
    - Here is a link to the camera we're considering:
        https://www.amazon.com/HiLetgo-OV7670-640x480-0-3Mega-Arduino/dp/B07S66Y3ZQ/ref=pd_sbs_328_3/139-5662391-1864915?_encoding=UTF8&pd_rd_i=B07S66Y3ZQ&pd_rd_r=02b49503-0e48-430b-b970-463fb46d19bc&pd_rd_w=8EE1m&pd_rd_wg=HRGGW&pf_rd_p=ed1e2146-ecfe-435e-b3b5-d79fa072fd58&pf_rd_r=08M03GN35VDEACAYYGNG&psc=1&refRID=08M03GN35VDEACAYYGNG


# Minutes 11/8/2020

We started the meeting with discussing a presentable document for the image processing algorithm
- Mathew will be working on this

We had a demo by Cody of the Sorter (sort of) sorting
- There are still some bugs to work out, the machine is moving between extremes

We discussed getting the presentation prepared
- Things to show while talking
- Videos and Documents to discuss
- Start with a video
- Each team creates a slide
- Future developments
- Team organization
- Mechanical design
- PCB design
- Controls software

We set up the sorter project presentation event invitation through IEEE vTools
- Register here! https://events.vtools.ieee.org/m/247537 


# Minutes 10/25/2020

We started our meeting this week with a presentation from Matt on the image processing
- He successfully coded a working algorithm in Matlab
- Next steps are to get some pictures of skittles that will reflect the final output to test the algorithm, and coding the algorithm in C
- We discussed how the camera interfaces with the Tm4C

Cody is working on the final touches for the mechanical design
- Depending on how complex importing 3D models from 360 to Blender will be, Jaidon will make an animation of the sorter


# Minutes 10/18

Mechanical design
    - Cody has had software issues with Fusion 360, so he is considering buying some aluminum extrusion to finish up

User Interface
    - For the user interface, we are aiming for some status lights
        - Idle, run, pause, stop
    - And 3 buttons
        - Start, pause, and E-stop

Parts Distribution
    - Amy has the funds to send parts out if anyone needs any

Pictures for the Easterner
    - We spent a good amount of the meeting taking some in-meeting pictures for the Easterner to use in the story about our club

Member list
    - We came up with an exhaustive list of club members, and estimated class years for each member


# Minutes from 10/4
We came up with a schedule to prepare for the sorter project presentation on November 18th at 5-6pm. The schedule was posted by Cody, here and on Discord.

We talked about a few ideas for the user interface:
- Using a mini industrial control panel
- Creating a PCB for it
- Attaching LEDs to some type of board

We discussed progress of the image processing:
- Possible use of a raspberry pi
- Issues with tm4c interfacing with camera

Upcoming Events:
- a capture the flag event on Tuesday with Gonzaga, time TBD.
- an educational cyber security event mentioned by Stu (talk to Amy for more info.)
- IEEE day on Tuesday October 6th 11-1pm: come see guest speakers, and participate in a competition of ideas.
- Career center meeting Thursday at 2pm
- A visitor from Easterner magazine is coming to talk during the meeting on Wednesday.

Special thanks to John for signing up to IEEE!



# Minutes from 9/27
We discussed the following miscellaneous:
* Image processing projects, and some examples of OCR with Python were shown by yours truly (Jaidon)
* Possibility of changing meeting times: Friday, or later Tuesday?
* Marketing club meetings and IEEE in classes
* Possibility of PLC projects and remote collaboration on code
* Stu possibly talking about cyber security and iot

We talked a bit about parts distribution:
* Parts list surveys
* Prebuilt kits
* Tm4c and basic parts for all

And the following about the sorter specifically:
* PCB progress (completed and functional)
* Cody will try to benchtest this week
* Cody and Nhat will collaborate on agitator
* 3D printing logistics issues (shipping costs, time)
* Jaidon will package PCBs for shipping
* Ian possibly collaborating with Mathew

Below are upcoming club events:
* Soldering tutorial Wednesday at 6
* EWU IEEE story by Easterner magazine in progress
* SAIL meeting Wednesday

We discussed the following possible new projects:
* Weather station
* Music and sound
* Guitar pedals
* Cyber security
* Halloween project
* Lighting driver

Attendance:
* Jaidon
* Chris
* Cody
* Nhat
* Mathew

Ieee members:
* Jaidon
* Chris
* Cody
* Amy
* Nhat
* Michael
