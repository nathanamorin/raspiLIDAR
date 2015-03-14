# This project is created with the goal of designing a low cost LIDAR system using a Raspberry Pi and off the shelf components.

## It was created as part of an Honors Project for CNIT 315 at Purdue University.  More information about the project is below.



**CNIT 315 HONORS PROJECT SPRING 2015**

&nbsp;

**Goal:**

To create a low cost LIDAR system using a off the shelf webcam and laser device. &nbsp;The goal is to create a system that could be used to map a room with sufficient accuracy to guide a robotic vehicle through with minimal cost and equipment overhead.

&nbsp;

**Physical Components:**

- --Microcontroller proposed : Raspberry Pi&nbsp;
- --Camera System : Adafruit Camera Module for Raspberry Pi&nbsp;
- --Laser : Basic Line Laser Module with no range finding capacity&nbsp;
- --(possible) Low Gear Motor & Motor Control System&nbsp;

&nbsp;

**Objective:**

The objective of this project is to complete at least Milestone 1 of the project during this semester. &nbsp;Developing the initial algorithm to reliably find and measure the point distance will be a significant challenge. &nbsp;If this is successfully completed, then an axis motor can be added allowing the camera / laser system to measure a wider room area (Milestone 2). &nbsp;Finally, if all the above components are accomplished successfully, Milestones 3 & 4 create the framework needed to guide a robotic device through the scanned field.

&nbsp;

**Milestones:**

Milestone 1.1 - Design the camera / laser system and algorithm to accurately

measure a single point distance.

&nbsp;

Milestone 1.2 - Modify system to calculate point distances in a line using the full line

produced by line laser module with a single camera frame.

&nbsp;

Milestone 2 - Add motor microcontroller to the system allowing camera / laser to be rotated

and multiple point-line measurements to be made in a ~180 degree radius.

&nbsp;

Milestone 3 - Using the system developed above, develop a data structure to hold 3D point

value data with the goal of being able to guide a robot through virtual x y z point plot map.

&nbsp;

Milestone 4 - Integrate with robotic system to autonomously guide device through obstacles.

&nbsp;