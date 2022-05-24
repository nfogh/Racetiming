# Racetiming
This project is an attempt to create a semi-professional online race-timing system.

This is still work-in-progress. Any help would be great!

Right now, it is based on the Simultaneous RFID reader from Sparkfun, but other RFID
readers can also be used.

Docker is used to create a database and a webserver which one can use to create races
and add participants and numbers. RFID tags can be auto-generated.

# Setup

## Webserver
You will need to fill out the config file in www/config/config.ini before the site will work.
There is an example in www/config/config_example.ini

Trying the webserver is easy if you install docker and docker-compose. Just run the command:

docker-compose up

And it should build the db, phpmyadmin and www images and run them.

Go to port 80, and you should see the webpage. From there, you can use the admin page to
log on and create races and participants.

Ideally, you will want to do this on some cloud VM instance, so others can see their results.
It should be fairly easy to set up on most cloud VM providers. It has been tested on google cloud.

# Usage

## RFID readers
The hardware consists of a Sparkfun Simultaneous RFID reader, and a redboard programmed with the
firmware that is also available in this project.

## Webpage
First, you need to start the webserver.

Then you need to add some races and some participants, and assign some numbers to the participants.

Once this is done, you need to create an API key, so that you can talk to the database through the GUI app.

## Writing RFID tags
The GUI app contains an easy-to-use tool to quickly write the correct EPC code to RFID tags.
Click on the RFID Writer button. This brings up a dialog where you can select the RFID writer you want to use
to write tags.

The writer assumes that you have already attached RFID tags to your bibs.

Once the writer has been connected, you can proceed to write the first tag. Put the displayed number bib in front of the writer, and press
the "write and proceed" button. This will write the correct EPC to the tag on the bib. Proceed with the next, until all the numbers from
the race has been assigned.

## GUI app
In the gui app, you need to select the web service endpoint, and the API key. Press connect, and the list
of races you have access to should be in the dropdown list. Once you select a race, click on the "Get runners"
button, and the list of runners should appear.

On the "RFID readers" tab, you can select which RFID readers you have connected to your system. Depending on the
type of race you have (one lap, multiple-lap, etc), you will need one or more readers. Also, you need to assign
the correct function to each reader.
