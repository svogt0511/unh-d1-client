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
function unh_d1_client_courseIsAvailable_courseCode($course_code = '', $program_area = array()) {
  $ret = FALSE;

  if (!empty($course_code)) {
    $course = unh_d1_client_searchCourse_courseCode($course_code, $program_area);

    if (!empty($course)) {
      $ret = unh_d1_client_courseIsAvailable($course, $program_area);  
    }
  }
  
  return $ret;
}

function unh_d1_client_courseIsAvailable($course = NULL, $program_area = array()) {
  $ret = FALSE;

  if (!empty($course)) {
    if (array_key_exists('courseNumber', $course)) {
      $sections = unh_d1_client_searchCourseSections($course['courseNumber'], $program_area, 'full');

      if (!empty($sections)) {
        foreach($sections as $section) {
          if (unh_d1_client_sectionIsAvailable($section) ||
            unh_d1_client_sectionIsWaitlisted($section) ||
            unh_d1_client_sectionIsFutureOffering($section) ||
            unh_d1_client_sectionIsEnrollmentClosed($section)
          ) {
            $ret = TRUE;
            break;
          }
        }
      }
    }
  }

  return $ret;
}

/**
 * Given a section, returns the whether it is within the enrollment period.
 */
function unh_d1_client_sectionWithinEnrollmentPeriod($section = NULL) {
  $ret = FALSE;
  
  $now = new DateTime('now');  
  if (!empty($section)) {
    if (($now >= unh_d1_client_getSectionEnrollmentBeginDate($section)) && 
      ($now <= unh_d1_client_getSectionEnrollmentEndDate($section)))
    {
      $ret = TRUE;
    }
  }
  
  return $ret;
}


/**
 * Given a section, returns the whether it is before the enrollment period.
 */
function unh_d1_client_sectionPreEnrollmentPeriod($section = NULL) {
  $ret = FALSE;
  
  $now = new DateTime('now');  
  if (!empty($section)) {
    if ($now < unh_d1_client_getSectionEnrollmentBeginDate($section)) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the whether it is after the enrollment period.
 */
function unh_d1_client_sectionPostEnrollmentPeriod($section = NULL) {
  $ret = FALSE;
  
  $now = new DateTime('now');  
  if (!empty($section)) {
    if ($now > unh_d1_client_getSectionEnrollmentEndDate($section)) {
      $ret = TRUE;
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
    if (unh_d1_client_sectionWithinEnrollmentPeriod($section) &&
      (($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_AVAILABLE) ||
      ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_FUTURE_OFFERING)))
    {
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

  $now = new DateTime('now');  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if (unh_d1_client_sectionPreEnrollmentPeriod($section) &&
       ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_FUTURE_OFFERING)) 
    {
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
 * Given a section, returns the section enrollment is closed (TRUE OR FALSE).
 * 
 * @todo: check dates, as well as status.
 */
function unh_d1_client_sectionIsEnrollmentClosed($section = NULL) {
  $ret = FALSE;
  
  $now = new DateTime('now');    
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ((unh_d1_client_sectionPostEnrollmentPeriod($section) &&
      ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_ENROLLMENT_CLOSED)) ||
      ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_ENROLLMENT_CLOSED))
    {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns whether the section is online (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsOnline($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section)) {
    if (unh_d1_client_getsectionInstructionMethod($section) == UNH_D1_CLIENT_SECTION_ONLINE) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns whether the section is online, self-paced (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsOnlineSelfPaced($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section)) {
    if (unh_d1_client_sectionIsOnline($section)) {
      $section_schedules = unh_d1_client_getSectionSchedules($section);
      if (!empty($section_schedules)) {
        if (unh_d1_client_getSectionScheduleType($section_schedules[0]) == UNH_D1_CLIENT_SECTION_SCHEDULE_ONLINE_SELF_PACED) {
          $ret = TRUE;
        }
      } 
    }
  }
  return $ret;
}


/**
 * Implements legacy call to get the course section profile.
 *
 * Returns the array of sections for the course.  Otherwise returns NULL.
 */
function unh_d1_client_legacy_getCourseSectionProfile_v55($course_code = '', $information_level = 'full') {
  $ret = NULL;

  if (!empty($course_code)) {
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_COURSESECTIONPROFILE, 
      $course_code, 
      array('informationLevel'  =>  $information_level), 
      array(), 
      'GET'
    );

    // There could be some checking here, but mostly, best effort.
    if ($code == '200') {
      if (!empty($result['courseProfile']['sections'])) {
        $sections = $result['courseProfile']['sections']['section'];
        $ret = _unh_d1_client_idxed_array($sections);
      }
    }
  }

  return $ret;
}


