/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : posthandler
 * @created     : Sunday Oct 27, 2019 11:56:20 MDT
 */

#include "posthandler.h"

extern pthread_mutex_t lock_main;

/*
 * Set the time stamp string of a tagpoint
 */
static int
set_time_stamp(tagpoint* tag)
{
  errno = 0;

  // Error check params
  if (!tag) {
    errno = EINVAL;
    perror("Invalid tag passed ");
    return errno;
  }

  // Get the current time
  time_t ltime;
  ltime = time(NULL);

  struct tm tm = *localtime(&ltime);
  sprintf(tag->timestamp, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

  size_t timestamp_length = strlen(tag->timestamp) + 1;

  // Error check time buffer, shouldn't be false,
  // but good to check
  if (timestamp_length > TIME_HEADER_SIZE) {
    errno = ENOBUFS;
    perror("Time stamp buffer was overflowed ");
    return errno;
  }
  // Update the length of the time stamp
  tag->timestamp_length = timestamp_length;
  tag->timestamp[timestamp_length - 1] = '\0'; // asctime puts a \n on the end
  return errno;
}

/*
 * Set the username of a tagpoint
 */
static int
set_username(tagpoint* tag, const void* data)
{
  errno = 0;
  int len = strlen((const char*)data) + 1;
  if (!len || len > MAX_USERNAME_LENGTH) {
    errno = EINVAL;
    perror("Data is either too long or not correct type ");
    return errno;
  }
  tag->user_name_length = len;
  memcpy((void*)tag->user_name, data, len);
  tag->user_name[len - 1] = '\0';
  return errno;
}

/*
 * Create a new tagpoint
 */
int
create_tagpoint(tagpoint* tag, char* username)
{
  errno = 0;

  // Check for valid arguments
  if (!tag || !username) {
    errno = EINVAL;
    perror("Username or tag is not initialized ");
    return errno;
  }

  // Initialize tag
  memset(tag, 0, sizeof(tagpoint));

  if (set_username(tag, username)) {
    return errno;
  }

  if (set_time_stamp(tag)) {
    return errno;
  }
  clock_gettime(CLOCK_MONOTONIC_RAW, &tag->index);
  srand(time(NULL));
  tag->userid = rand();
  return errno;
}

int
tagpoint_setopt(tagpoint* point, TAGPOINT_OPT_T option, const void* data)
{
  errno = 0;
  switch (option) {
    case (USERNAME):
      return set_username(point, data);
    case (TAGPOINT_POINT):
      point->tag_options ^= TAG_POINT;
      update_tag_point_timer(point);
      return errno;
    case (TAGPOINT_LOST):
      point->tag_options &= ~TAG_POINT;
      return errno;
    case (MICROSECONDS):
    case (MILLISECONDS):
    case (SECONDS):
    case (DECISECONDS):
    case (MINUTES):
      point->tag_options |= (TIME_MASK | option);
      return errno;
  }
  errno = ENOSYS;
  perror("No implimentation for option ");
  return errno;
}

/*
 * Assumes we are seeing a tag, updates the time
 */
int
incriment_tag_point_timer(tagpoint* point)
{
  errno = 0;

  if (!point) {
    errno = EINVAL;
    perror("Invalid point parameter ");
    return errno;
  }

  // Get the previous time, recorded by a call to incriment tag_point timer
  uint64_t previous_time = point->index.tv_sec * 1000000000 +
                           point->index.tv_nsec * 1000; // nanoseconds

  // Change point-.index to now
  clock_gettime(CLOCK_MONOTONIC_RAW, &point->index);

  // Current time
  uint64_t curr_time = point->index.tv_sec * 1000000000 +
                       point->index.tv_nsec * 1000; // microseconds

  // Convert to units based on options
  if (point->tag_options & MICROSECONDS) {
    point->dur += (curr_time - previous_time) / 1000;
  } else if (point->tag_options & MILLISECONDS) {
    point->dur += (curr_time - previous_time) / 1000000;
  } else if (point->tag_options & SECONDS) {
    point->dur += (curr_time - previous_time) / 1000000000;
  } else if (point->tag_options & DECISECONDS) {
    point->dur += (curr_time - previous_time) / 10000000000;
  } else if (point->tag_options & MINUTES) {
    point->dur += (curr_time - previous_time) / 60000000000;
  } else {
    printf("here\n");
    point->dur += (curr_time - previous_time) / 1000000000;
  }

  return errno;
}

// Called when tag point is set
// simply syncs the timer and zeros the duration
int
update_tag_point_timer(tagpoint* point)
{
  errno = 0;
  if (!point) {
    errno = EINVAL;
    perror("Invalid point pointer ");
    return errno;
  }
  point->dur = 0;
  clock_gettime(CLOCK_MONOTONIC_RAW, &point->index);
  return errno;
}

int
update(tagpoint* point, int tag_seen)
{
  if (tag_seen == TAGPOINT_POINT) {
    tagpoint_setopt(point, tag_seen, NULL);
  } else if (tag_seen == TAGPOINT_LOST) {
    tagpoint_setopt(point, tag_seen, NULL);
  }

  if (point->tag_options & TAG_POINT)
    incriment_tag_point_timer(point);
  return 1;
}

int
tag_point_to_post_request(tagpoint* point, char* destination)
{
  errno = 0;
  if (!point || !destination || !(destination + POST_MAX_LENGTH)) {
    errno = EINVAL;
    perror("Invalid argument ");
    return errno;
  }

  if (set_time_stamp(point)) {
    return errno;
  }

  if (incriment_tag_point_timer(point)) {
    return errno;
  }

  sprintf(destination,
          "userid=%d&username=%s&timestamp=%s&tagtime=%ld",
          point->userid,
          point->user_name,
          point->timestamp,
          point->dur);

  size_t l = strlen(destination);
  destination[l] = '\0';

  return errno;
}
