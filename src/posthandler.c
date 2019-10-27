/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : posthandler
 * @created     : Sunday Oct 27, 2019 11:56:20 MDT
 */

#include "posthandler.h"

int create_tagpoint(tagpoint* tag, char* username) {
  errno = 0;
  if(!tag || !username){
    errno = EINVAL;
    perror("Username or tag is not initialized ");
    return errno;
  }

  size_t user_name_length = strlen(username);
  if(user_name_length > MAX_USERNAME_LENGTH) {
    errno = EINVAL;
    perror("Username is too long ");
    return errno;
  }

  tag->user_name_length = user_name_length;

  time_t ltime;
  ltime = time(NULL);

  snprintf(tag->timestamp, TIME_HEADER_SIZE, "%s", asctime(localtime(&ltime)));
  size_t timestamp_length = strlen(tag->timestamp);

  if(timestamp_length > TIME_HEADER_SIZE) {
    errno = ENOBUFS;
    perror("Time stamp buffer was overflowed ");
    return errno;
  }

  tag->timestamp_length = timestamp_length;

  return errno;
}

static int set_point_successful(tagpoint* tag, const void* data) {
  if(!(const uint8_t*)data + sizeof(TAGPOINT)){
    errno = EINVAL;
    perror("Data is not a valid TAGPOINT type ");
    return errno;
  }
  tag->point |= *(TAGPOINT*)data;
  return errno;
}

static int set_username(tagpoint* tag, const void* data) {
  int len = strlen((const char*)data);
  if(!len || len > MAX_USERNAME_LENGTH){
    errno = EINVAL;
    perror("Data is either too long or not correct type ");
    return errno;
  }
  tag->point |= *(TAGPOINT*)data;
  return errno;
}

int tagpoint_setopt(tagpoint* point, TAGPOINT_OPT option, const void* data) {
  errno = 0;
  switch(option) {
    case(USERNAME):
      return set_username(point, data);
    case(TAGPOINT_POINT):
      return set_point_successful(point, data);
  }
  errno = ENOSYS;
  perror("No implimentation for option %d ", option);
  return errno;
}
