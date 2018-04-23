<?php

/**
 * @file
 *
 * @todo: Update this docblock.
 * @todo: make sure we get full output on other than 'short' informationLevel
 *
 * Functions/'methods' get data from D1 using the web services.
 * For ex, get lists of things like: course lists, section lists, etc.
 *
 * Also functions/'methods' for getting information from the data structures
 * returned from D1, like determining course status, section status, etc.
 */

/******************************************************************************
 *
 * Functions to extract data of interest from the API.
 *
 ******************************************************************************/


/******************************************************************************
 *
 * Functions to determine STATUSES of various data objects returned by the API.
 *
 * This is the 5.5 version.
 *
 ******************************************************************************/

/**
 * Given a course, returns the course availability (TRUE OR FALSE).
 *
 * Courses are available if:
 *
 *   - there is 1 or more scheduled sections that:
 *       - Are publically available ('returnOnlyAvailableSectionsFlag = true). Default.
 *       - Have scheduled sections('searchOnlyScheduledCoursesFlag = 'true'). Default.
 *       - At least one of the scheduled sections is 'available'.
 *
 * Section is available if:
 *   - section status is "available" || "waitlist_available".
 * 
 */
function unh_d1_client_courseIsAvailable($course = NULL) {
  // for testing with 5.4 @todo
  return TRUE;
  $ret = FALSE;
  
  if (!empty($course) && array_key_exists('courseNumber', $course)) {
    $sections = unh_d1_client_searchCourseSections($course['courseNumber']);
    
    if (!empty($sections)) {
      foreach($sections as $section) {
        if (unh_d1_client_sectionIsAvailable($section) ||
          unh_d1_client_sectionIsWaitlisted($section)
        ) {
          $ret = TRUE;
          break;
        }
      }
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section availability (TRUE OR FALSE).
 *
 * @todo: set to return true/false depending on if API version is 5.4 or 5.5
 */
function unh_d1_client_sectionIsAvailable($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_AVAILABLE) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section is no longer available (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsNoLongerAvailable($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_NO_LONGER_AVAILABLE) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section is waitlisted (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsWaitlisted($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_WAITLIST_AVAILABLE) {
      $ret = TRUE;
    }
  }
  return $ret;
}

/**
 * Given a section, returns the section is a future offering (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsFutureOffering($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_FUTURE_OFFERING) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section is full (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsFull($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_FULL) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section is waitlisted (TRUE OR FALSE).
 * 
 * @todo: check dates, as well as status.
 */
function unh_d1_client_sectionIsEnrollmentClosed($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_ENROLLMENT_CLOSED) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}