# Racetiming
Very simple running race timing application written in QT.

Buggy as hell :D

# Usage
It is so simple it should be pretty much self-explanatory.

Just add your participants and start their time.

You can save the participant list to a json file and load it later.

To do post-processing of the time, use the "save timing" menu option. It saves it to .csv file.

# Screenshots
![](./Doc/Racetiming.png)

# Docker
Trying the webserver is easy if you install docker. Just run the command:

docker-compose up

And it should build the db, phpmyadmin and www images and run them.

You will need to fill out the config file in www/config/config.ini before the site will work.
There is an example in www/config/config_example.ini
