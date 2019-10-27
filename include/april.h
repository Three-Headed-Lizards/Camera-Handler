/* Copyright (C) 2013-2016, The Regents of The University of Michigan.
All rights reserved.
This software was developed in the APRIL Robotics Lab under the
direction of Edwin Olson, ebolson@umich.edu. This software may be
available under alternative licensing terms; contact the address above.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the Regents of The University of Michigan.
*/

#include <iostream>

#include "opencv2/opencv.hpp"

extern "C"
{
#include <apriltag/apriltag.h>
#include <apriltag/common/getopt.h>
#include <apriltag/tag16h5.h>
#include <apriltag/tag25h9.h>
#include <apriltag/tag36h11.h>
#include <apriltag/tagCircle21h7.h>
#include <apriltag/tagCircle49h12.h>
#include <apriltag/tagCustom48h12.h>
#include <apriltag/tagStandard41h12.h>
#include <apriltag/tagStandard52h13.h>
}

#define TAG_1 "tag36h11"
#define TAG_2 "tag25h9"
#define TAG_3 "tag16h5"
#define TAG_4 "tagCircle21h7"
#define TAG_5 "tagCircle49h12"
#define TAG_6 "tagStandard41h12"
#define TAG_7 "tagStandard52h13"
#define TAG_8 "tagCustom48h12"

/**
 * @brief Contain everything related to the tag in one struct
 *
 * Tag family is the type of tag that we pass as a parameter
 * april tags come in families of accuracies
 *
 * Tag detector is the handel to detect the april tag
 *
 * video capture is opencv's way of creating video capture
 *
 * detecting is set to the number of tags detected at any given point in time
 * in the obtain image loop
 */
typedef struct
{
  apriltag_family_t* tf;
  apriltag_detector_t* td;
  char famname_dest[100];
  cv::VideoCapture cap;
  cv::Mat frame;
  cv::Mat gray;
  int detecting;
} tag_properties;

/**
 * @brief Parses the command line to get arguments, tag type etc
 *
 * @param arc argc from command line
 * @param argv argv from the command line
 * @param famname_dest The destination for the tag type name
 * @param properties The april tag is initialized when command parameters are
 * passed
 *
 * @return Status of the parse
 */
int
parse_command_line(int arc,
                   char** argv,
                   char* famname_dest,
                   tag_properties* properties);

int
create_april_tag(apriltag_family_t* tf, const char* famname);

int
capture_april_tags(apriltag_family_t* tf);
