<?php

/**
 * @file
 *
 * @todo: Update this docblock.
 *
 * Functions/'methods' get data from D1 using the web services.
 * For ex, get lists of things like: course lists, section lists, etc.
 *
 * Also functions/'methods' for getting information from the data structures
 * returned from D1, like determining course status, section status, etc.
 */

/******************************************************************************
 *
 * Calls to extract data of interest from the API.
 *
 * All calls are made relative to the configured program area.
 *
 ******************************************************************************/

/**
 * Implements a course search with the given keyword/text.
 *
 * Returns an array of courses that match the specifications.  If no courses
 * match specifications, returns an empty array.
 *
 * Searches all courses in the configured program area.
 */
function unh_d1_client_searchCourse_keyword($keyword = '', $information_level = 'short') {
  $ret = array();
  
  if (!empty($keyword)) {
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_SEARCHCOURSE, '', 
      array('informationLevel'  =>  'short'), 
      array('keyword' => $keyword), 'POST'
    );
    
    // There could be some checking here, but mostly, best effort.
    if ($code == '200') {
      $courses = $result['searchCourseResult']['courseProfiles']['courseProfile'];
      $ret = _unh_d1_client_idxed_array($courses);
    }
  }

  return $ret;
}


/**
 * Implements a course search with the given course code.
 *
 * Returns a course if there is one with the given course code.  Otherwise returns NULL.
 */
function unh_d1_client_searchCourse_course_code($course_code = '', $information_level = 'short') {
  $ret = NULL;
  
  if (!empty($course_code)) {
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_SEARCHCOURSE, '', 
      array('informationLevel'  =>  'short'), 
      array('courseCode' => $course_code), 'POST'
    );
    
    // There could be some checking here, but mostly, best effort.
    if ($code == '200') {
      $course = $result['searchCourseResult']['courseProfiles']['courseProfile'];
      $ret = $course;
    }
  }

  return $ret;
}


/**
 * Gets the sections belonging to a course
 *
 * Returns an array of courses.  Searches all courses in the configured program area.
 */
function unh_d1_client_searchCourseSections_course_code($course_code = '', $information_level = 'short') {
  $ret = array();
  
  if (!empty($course_code)) {
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_SEARCHCOURSESECTION, '', 
      array('informationLevel'  =>  'short'), 
      array('courseCode' => $course_code), 'POST'
    );
    // There could be some checking here, but mostly, best effort.
    if ($code == '200') {
      $sections = $result['SearchCourseSectionProfileResult']['courseSectionProfiles']['courseSectionProfile'];
      $ret = _unh_d1_client_idxed_array($sections);
    }
  }

  return $ret;
}


