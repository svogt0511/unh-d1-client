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

/**
 * Implements a course search with the given keyword/text.
 *
 * Returns an array of courses that match the specifications.  If no courses
 * match specifications, returns an empty array.
 *
 * Searches all courses in the configured program area.
 */
function unh_d1_client_searchCourse_keyword($keyword = '', $programAreas = array(), $information_level = 'short') {
  $ret = array();
  
  if (!empty($keyword)) {
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_SEARCHCOURSE, 
      '', 
      array('informationLevel'  =>  $information_level), 
      array('keyword' => $keyword, 'programAreas' => $programAreas), 
      'POST'
    );
    
    // There could be some checking here, but mostly, best effort.
    if ($code == '200') {
      if (!empty($result['searchCourseResult']['courseProfiles'])) {
        $courses = $result['searchCourseResult']['courseProfiles']['courseProfile'];
        $ret = _unh_d1_client_idxed_array($courses);
      }
    }
  }

  return $ret;
}


/**
 * Implements a course search with the given course code.
 *
 * Returns one course if there is one with the given course code.  Otherwise returns NULL.
 */
function unh_d1_client_searchCourse_courseCode($course_code = '', $programAreas = array(), $information_level = 'short') {
  $ret = NULL;

  if (!empty($course_code)) {
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_SEARCHCOURSE, 
      '', 
      array('informationLevel'  =>  $information_level), 
      array('courseCode' => $course_code, 'programAreas' => $programAreas), 
      'POST'
    );
    // There could be some checking here, but mostly, best effort.
    if ($code == '200') {
      if (!empty($result['searchCourseResult']['courseProfiles'])) {
        $course = $result['searchCourseResult']['courseProfiles']['courseProfile'];
        // Returning at most 1 course. (Don't use _unh_d1_client_idxed_array.)
        $ret = $course;
      }
    }
  }

  return $ret;
}

/**
 * Gets specified groups of courses in the given program areas. 
 *
 * Returns a list of all courses within the configured program area.
 * Default - returns all courses in all program areas.
 *
 * If no criteria are specified - no courses are returned.  However,
 * if given a list of all program areas, will return all courses.
 */
function unh_d1_client_searchCourse_programAreas($programAreas = array(), $information_level = 'short') {
  $ret = array();
  
  $options = array('informationLevel'  =>  $information_level);
  $criteria = array('programAreas' => $programAreas);
  list($code, $error, $result) = unh_d1_client_request(
    UNH_D1_CLIENT_SEARCHCOURSE, 
    '', 
    $options, 
    $criteria, 
    'POST'
  );
  // There could be some checking here, but mostly, best effort.
  if ($code == '200') {
    $courses = $result['searchCourseResult']['courseProfiles']['courseProfile'];
    $ret = _unh_d1_client_idxed_array($courses);
  }

  return $ret;
}

/**
 * Gets the sections belonging to a course
 *
 * Returns an array of courses.
 *
 * @todo - make a loop to get paged results.
 */
function unh_d1_client_searchCourseSections($course_code = '', $programAreas = array(), $information_level = 'short', $flags = array()) {
  $ret = array();
    
  if (!empty($course_code)) {
    $more_to_get = TRUE;
    $i = 1;

    while ($more_to_get) {
      $options = array('informationLevel'  =>  $information_level);
      $criteria = array('courseCode' => $course_code, 'programAreas' => $programAreas, 'paginationConstruct' => ['pageNumber' => $i, 'pageSize' => UNH_D1_CLIENT_MAX_PAGE_SIZE]);
    
      foreach($flags as $key => $flag) {
        if (in_array($key, UNH_D1_CLIENT_API_REQUESTS[UNH_D1_CLIENT_SEARCHCOURSESECTION]['requestCriteria'])) {
          $criteria[$key] = $flag;
        }
      }
    
      //list($code, $error, $result) = unh_d1_client_request_full(
      list($code, $error, $result) = unh_d1_client_request(
        UNH_D1_CLIENT_SEARCHCOURSESECTION, 
        '', 
        $options, 
        $criteria, 
        'POST'
      );

      // There could be some checking here, but mostly, best effort.
      if ($code == '200') {
        $sections = $result['SearchCourseSectionProfileResult']['courseSectionProfiles']['courseSectionProfile'];
        $ret = $ret + _unh_d1_client_idxed_array($sections);
        $pagination_response = unh_d1_client_get_pagination_response($result);
        if (unh_d1_client_got_full_response($pagination_response)){
          $more_to_get = FALSE;
          break;
        }
      // Error
      } else {
        $more_to_get = FALSE;
        break;
      }
      
      $i++;
    }
  }

  return $ret;
}

/**
 * Gets the section belonging to a course with given section number.
 *
 * Returns an array of courses.
 */
function unh_d1_client_searchCourseSection($course_code = '', $custom_section_number = '', $programAreas = array(), $information_level = 'short', $flags = array()) {
  $ret = array();
  
  if (!empty($course_code)) {
    
    $options = array('informationLevel'  =>  $information_level);
    
    $criteria = array('courseCode' => $course_code, 'programAreas' => $programAreas);
    
    if (!empty($custom_section_number)) {
      $criteria['customSectionNumber'] = $custom_section_number;
    }
    
    foreach($flags as $key => $flag) {
      if (in_array($key, UNH_D1_CLIENT_API_REQUESTS[UNH_D1_CLIENT_SEARCHCOURSESECTION]['requestCriteria'])) {
        $criteria[$key] = $flag;
      }
    }
    
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_SEARCHCOURSESECTION, 
      '', 
      $options, 
      $criteria, 
      'POST'
    );

    // There could be some checking here, but mostly, best effort.
    // There should only be 1 section.
    if ($code == '200') {
      $result = $result['SearchCourseSectionProfileResult']['courseSectionProfiles']['courseSectionProfile'];
      $sections = _unh_d1_client_idxed_array($result);
      $ret = $sections[0];
    }
  }

  return $ret;
}

/**
 * Returns a list of programAreas.
 *
 * Gets the list through the programAreas call.
 */
function unh_d1_client_getprogramAreas($information_level = 'short') {
  $ret = array();
  
  $options = array('informationLevel'  =>  $information_level);
  $criteria = array();
  list($code, $error, $result) = unh_d1_client_request(
    UNH_D1_CLIENT_PROGRAMAREAS, 
    '', 
    $options, 
    $criteria, 
    'GET'
  );
  
  // There could be some checking here, but mostly, best effort.
  if ($code == '200') {
    $programAreas = $result['programArea'];
    $ret = _unh_d1_client_idxed_array($programAreas);
  }

  return $ret;
}


/**
 * Returns a list of programAreas (v1). 
 *
 * Gets the list through the baseEntities call.
 */
function unh_d1_client_getprogramAreasV1($information_level = 'short') {
  $ret = array();
  
  $options = array('informationLevel'  =>  $information_level);
  $criteria = array();
  list($code, $error, $result) = unh_d1_client_request(
    UNH_D1_CLIENT_BASEENTITIES, 
    'programAreas', 
    $options, 
    $criteria, 
    'GET'
  );
  
  // There could be some checking here, but mostly, best effort.
  if ($code == '200') {
    $programAreas = $result['baseEntity'];
    $ret = _unh_d1_client_idxed_array($programAreas);
  }

  return $ret;
}

/**
 * Returns a list of semesters.
 *
 * Gets the list through the semesters call.
 */
function unh_d1_client_getsemesters($information_level = 'short') {
  $ret = array();
  
  $options = array('informationLevel'  =>  $information_level);
  $criteria = array();
  list($code, $error, $result) = unh_d1_client_request(
    UNH_D1_CLIENT_SEMESTERS, 
    '', 
    $options, 
    $criteria, 
    'GET'
  );
  
  // There could be some checking here, but mostly, best effort.
  if ($code == '200') {
    $semesters = $result['semester'];
    $ret = _unh_d1_client_idxed_array($semesters);
  }

  return $ret;
}

/**
 * Given a program area, returns an array of certificates from that programArea.
 */
function unh_d1_client_getCertificates($programArea = '', $information_level = 'short') {
  $ret = array();
  
  $options = array('informationLevel'  =>  $information_level);
  $criteria = array();
  
  list($code, $error, $result) = unh_d1_client_request(
    UNH_D1_CLIENT_CERTIFICATES, 
    '', 
    $options, 
    $criteria, 
    'GET'
  );
  
  // There could be some checking here, but mostly, best effort.
  if ($code == '200') {
    $certificates = $result['certificate'];
    $certificates = _unh_d1_client_idxed_array($certificates);
    
    foreach($certificates as $certificate) {
     // if (unh_d1_client_getCertificateProgramAreaCode($certificate) == $programArea) {
        $ret[] = $certificate;
     // }
    }
  }

  return $ret;
}

/******************************************************************************
 *
 * Functions to extract data from CERTIFICATE objects obtained from the API.
 *
 ******************************************************************************/
 
/**
 *
 * Given a certificate, returns the 'programAreaCode'.
 * 
 */
function unh_d1_client_getCertificateProgramAreaCode($certificate = NULL) {
  $ret = '';
  
  if (!empty($certificate) && array_key_exists('programArea', $certificate)) {
    $ret = $certificate['programArea']['code'];
  }
  
  return $ret;
}

/******************************************************************************
 *
 * Functions to extract data from COURSE objects obtained from the API.
 *
 ******************************************************************************/
 
/**
 *
 * Given a course, returns the 'name'.
 * 
 */
function unh_d1_client_getcourseName($course = NULL) {
  $ret = '';
  
  if (!empty($course) && array_key_exists('name', $course)) {
    $ret = $course['name'];
  }
  
  return $ret;
}
 
/**
 *
 * Given a course, returns the 'courseNumber'.
 * 
 */
function unh_d1_client_getcourseNumber($course = NULL) {
  $ret = '';
  
  if (!empty($course) && array_key_exists('courseNumber', $course)) {
    $ret = $course['courseNumber'];
  }
  
  return $ret;
}

/**
 *
 * Given a course, returns the 'officialCourseDescriptionPublic'.
 * 
 */
function unh_d1_client_getcourseOverview($course = NULL) {
  $ret = '';
  
  if (!empty($course) && array_key_exists('officialCourseDescriptionPublic', $course)) {
    $ret = $course['officialCourseDescriptionPublic'];
  }
  
  return $ret;
}
 
/**
 *
 * Given a course, returns the 'objectId'.
 * 
 */
function unh_d1_client_getcourseObjectId($course = NULL) {
  $ret = '';
  
  if (!empty($course) && array_key_exists('objectId', $course)) {
    $ret = $course['objectId'];
  }
  
  return $ret;
}


/******************************************************************************
 *
 * Functions to extract data from SECTION objects obtained from the API.
 *
 ******************************************************************************/

/**
 *
 * Given a section, returns the 'sectionTitle'.
 * 
 */
function unh_d1_client_getsectionTitle($section = NULL) {
  $ret = '';
  
  if (!empty($section) && array_key_exists('sectionTitle', $section)) {
    $ret = $section['sectionTitle'];
  }
  
  return $ret;
}

/**
 *
 * Given a section, returns the 'customSectionNumber'.
 * 
 */
function unh_d1_client_getsectionCustomSectionNumber($section = NULL) {
  $ret = '';
  
  if (!empty($section) && array_key_exists('customSectionNumber', $section)) {
    $ret = $section['customSectionNumber'];
  }
  return $ret;
}

/**
 *
 * Given a section, returns an array of the section 'discounts'.
 * 
 */
function unh_d1_client_getsectionDiscounts($section = NULL) {
  $ret = array();
  
  if (!empty($section) && array_key_exists('discounts', $section)) {
    $ret = _unh_d1_client_idxed_array($section['discounts']['discount']);
  }
  
  return $ret;
}

/**
 *
 * Given a section, returns an array of discount names.
 * 
 */
function unh_d1_client_getsectionDiscountNames($section = NULL) {
  $ret = array();
  
  if (!empty($section)) {
    $discounts = unh_d1_client_getsectionDiscounts($section);
    
    if (!empty($discounts)) {
      foreach($discounts as $discount) {
        if ($discount['publicApplicable']) {
          $ret[] = $discount['printCode'];
        }
      }
    }
  }
  
    return $ret;
}


/**
 *
 * Given a section, returns the 'contact hours' (sectionDenormInfo['calculatedTotalHours]).
 * 
 */
function unh_d1_client_getsectionContactHours($section = NULL) {
  $ret = '';

  if (!empty($section) && array_key_exists('sectionDenormInfo', $section)) {
    $ret = $section['sectionDenormInfo']['calculatedTotalHours'];
  }

  return $ret;
}


/**
 *
 * Given a section, returns the 'CEUs' (sectionDenormInfo['calculatedTotalHours]).
 * 
 */
function unh_d1_client_getsectionCEUs($section = NULL) {
  $ret = '';

  if (!empty($section) && array_key_exists('maxCEUnit', $section)) {
    $ret = $section['maxCEUnit'];
  }

  return $ret;
}

/**
 *
 * Given a section, returns the 'sectionAvailabilityCode'.
 * 
 */
function unh_d1_client_getsectionAvailabilityCode($section = NULL) {
  $ret = '';
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    $ret = $section['sectionAvailabilityCode'];
  }
  
  return $ret;
}

/**
 *
 * Given a section, returns the 'associatedTerm' name.
 * 
 */
function unh_d1_client_getsectionSemester($section = '') {
  $ret = '';
  
  if (!empty($section) && array_key_exists('associatedTerm', $section)) {
    $ret = $section['associatedTerm']['name'];
  }
  
  return $ret;
}

/**
 *
 * Given a section, returns the 'objectId'.
 * 
 */
function unh_d1_client_getsectionObjectId($section = NULL) {
  $ret = null;
  
  if (!empty($section) && array_key_exists('objectId', $section)) {
    $ret = $section['objectId'];
  }
  return $ret;
}

/**
 *
 * Given a section, returns the 'pvEnrollmentBeginDate' name.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionEnrollmentBeginDate($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('pvEnrollmentBeginDate', $section)) {
    $ret = new DateTime($section['pvEnrollmentBeginDate']);
  }
  return $ret;
}

/**
 *
 * Given a section, returns the 'pvEnrollmentEndDate' name.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionEnrollmentEndDate($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('pvEnrollmentEndDate', $section)) {
    $ret = new DateTime($section['pvEnrollmentEndDate']);
  }
  return $ret;
}

/**
 *
 * Given a section, returns the 'pvAvailabilityBeginDate' name.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionPvAvailaibilityBeginDate($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('pvAvailabilityBeginDate', $section)) {
    $ret = new DateTime($section['pvAvailabilityBeginDate']);
  }
  return $ret;
}

/**
 *
 * Given a section, returns the 'pvAvailabilityEndDate' name.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionPvAvailabilityEndDate($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('pvAvailabilityEndDate', $section)) {
    $ret = new DateTime($section['pvAvailabilityEndDate']);
  }
  return $ret;
}


/**
 *
 * Given a section, returns the instruction method.  Assumption is that there is only 1 method of instruction.
 *
 * Test courses would be on D1 test server: 
 *
 *   Using Technology to Create Online Courses and Presentations(PDT-ComAP-04)
 *   Animal-Assisted Therapy(DT-SocWk-02)
 *
 * d1 prod server:
 *
 *   Animal-Assisted Therapy (PDT-SocWk-02)
 *   Brain-based Basics (PDT-TchGN-32)
 * 
 */
function unh_d1_client_getsectionInstructionMethod($section = NULL) {
  $ret = '';  
    
  if (!empty($section) && array_key_exists('InstructionMethods', $section)) {
    $ret = $section['InstructionMethods']['instructionMethod']['description'];
  }
  
  return $ret;
}

/**
 *
 * Given a section, returns the 'sectionNotes'.
 * 
 */
function unh_d1_client_getsectionNotes($section = NULL) {
  $ret = '';
  
  if (!empty($section) && array_key_exists('sectionNotes', $section)) {
    $ret = $section['sectionNotes'];
  }
  
  return $ret;
}



/******************************************************************************
 *
 * Functions to extract data from SECTION SCHEDULE objects obtained from the API.
 *
 ******************************************************************************/

/**
 *
 * Given a section, returns an array of 'sectionSchedules'.
 *
 */
function unh_d1_client_getSectionSchedules($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('sectionSchedules', $section)) {
    $ret = _unh_d1_client_idxed_array($section['sectionSchedules']['sectionSchedule']);
  }
  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns a the start date.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionScheduleStartDate($sectionSchedule = NULL) {
  $ret = NULL;
  if (!empty($sectionSchedule) && array_key_exists('startDate', $sectionSchedule) && !empty($sectionSchedule['startDate'])) {
    $ret = new DateTime($sectionSchedule['startDate']);
  }
  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns the end date.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionScheduleEndDate($sectionSchedule = NULL) {
  $ret = NULL;
  if (!empty($sectionSchedule) && array_key_exists('endDate', $sectionSchedule) && !empty($sectionSchedule['endDate'])) {
    $ret = new DateTime($sectionSchedule['endDate']);
  }
  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns a the start time.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionScheduleStartTime($sectionSchedule = NULL) {
  $ret = NULL;
  if (!empty($sectionSchedule) && array_key_exists('startTimeString', $sectionSchedule) && !empty($sectionSchedule['startTimeString'])) {
    // $ret = new DateTime($sectionSchedule['startTimeString']);
    $time_str = _unh_d1_client_cv_time_string_sectionSchedule($sectionSchedule['startTimeString']);
    $ret = DateTime::createFromFormat(UNH_D1_DRUPAL_SECTION_SCHEDULE_TIME_FORMAT, $time_str);
  }
  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns the end time.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionScheduleEndTime($sectionSchedule = NULL) {
  $ret = NULL;
  if (!empty($sectionSchedule) && array_key_exists('endTimeString', $sectionSchedule) && !empty($sectionSchedule['endTimeString'])) {
    // $ret = new DateTime($sectionSchedule['endTimeString']);
    $time_str = _unh_d1_client_cv_time_string_sectionSchedule($sectionSchedule['endTimeString']);
    $ret = DateTime::createFromFormat(UNH_D1_DRUPAL_SECTION_SCHEDULE_TIME_FORMAT, $time_str);
  }
  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns an array of instructors.
 *
 */
function unh_d1_client_getSectionInstructors($sectionSchedule = NULL) {
  $ret = array();
  
  if (!empty($sectionSchedule) && array_key_exists('assignedInstructors', $sectionSchedule)) {
    $ret = _unh_d1_client_idxed_array($sectionSchedule['assignedInstructors']['assignedInstructor']);
  }

  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns an array of instructor names.
 *
 */
function unh_d1_client_getSectionInstructorNames($sectionSchedule = NULL) {
  $ret = array();

  if (!empty($sectionSchedule)) {
    $assignedInstructors = unh_d1_client_getSectionInstructors($sectionSchedule);

    if (!empty($assignedInstructors)) {
      foreach ($assignedInstructors as $instructor) {
        $instructor_name = '';
        if (!empty($instructor['instructor']['firstName1']) && !empty($instructor['instructor']['lastName'])) {
          $instructor_name = $instructor['instructor']['firstName1'] . ' ' . $instructor['instructor']['lastName'];      
        } elseif (!empty($instructor['instructor']['firstName1'])) {
          $instructor_name = $instructor['instructor']['firstName1'];
        } elseif (!empty($instructor['instructor']['lastName'])) {
          $instructor_name = $instructor['instructor']['lastName'];
        }

        if (!empty($instructor_name)) {
          $ret[] = $instructor_name;
        }
      }
    }
  }

  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns the campus/location name.
 *
 */
function unh_d1_client_getSectionCampusName($sectionSchedule = NULL) {
  $ret = '';

  if (!empty($sectionSchedule) && array_key_exists('campus', $sectionSchedule)) {
    $ret = $sectionSchedule['campus']['name'];
  }

  return $ret;
}


/**
 *
 * Given a sectionSchedule, returns the schedule type.
 *
 */
function unh_d1_client_getSectionScheduleType($sectionSchedule = NULL) {
  $ret = '';

  if (!empty($sectionSchedule) && array_key_exists('scheduleType', $sectionSchedule)) {
    $ret = $sectionSchedule['scheduleType'];
  }

  return $ret;
}


/******************************************************************************
 *
 * Functions to extract data from COURSE SECTION FEE objects obtained from the API.
 * In particular, we are interested in the first array of 'tuitionFeeItems' 
 * and its associated 'publisedCode'.
 *
 ******************************************************************************/

/**
 *
 * Given a section, returns an array of 'courseSectionFees'.
 *
 * N of these. (Assumption is 1 only.)
 */
function unh_d1_client_getCourseSectionFees($section = NULL) {
  $ret = array();
  if (!empty($section) && array_key_exists('courseSectionFees', $section)) {
    $ret = _unh_d1_client_idxed_array($section['courseSectionFees']['courseSectionFee']);
  }
  return $ret;
}

/**
 *
 * Given a courseSectionFee object, returns an array of 'associatedSectionFeeTuitionProfiles'.
 *
 * N of these each with published code.
 */
function unh_d1_client_getAssociatedSectionFeeTuitionProfiles($courseSectionFee = NULL) {
  $ret = array();
  if (!empty($courseSectionFee) && 
    array_key_exists('associatedSectionFeeTuitionProfiles', $courseSectionFee) && 
    !empty($courseSectionFee['associatedSectionFeeTuitionProfiles'])) 
  {
    $ret = _unh_d1_client_idxed_array($courseSectionFee['associatedSectionFeeTuitionProfiles']['associatedSectionFeeTuitionProfile']);
  }
  return $ret;
}

/**
 *
 * Given an associatedSectionFeeTuitionProfiles object, returns an 'associatedTuitionProfile'.
 *
 * 1 of these each with published code.
 */
function unh_d1_client_getAssociatedTuitionProfile($associatedSectionFeeTuitionProfile = NULL) {
  $ret = NULL;
  if (!empty($associatedSectionFeeTuitionProfile) && array_key_exists('associatedTuitionProfile', $associatedSectionFeeTuitionProfile)) {
    $ret = $associatedSectionFeeTuitionProfile['associatedTuitionProfile'];
  }
  return $ret;
}

/**
 *
 * Given an 'associatedTuitionProfile' object, returns an 'publishedCode'.
 *
 */
function unh_d1_client_getPublishedCode($associatedTuitionProfile = NULL) {
  $ret = '';
  if (!empty($associatedTuitionProfile) && array_key_exists('publishedCode', $associatedTuitionProfile)) {
    $ret = $associatedTuitionProfile['publishedCode'];
  }
  return $ret;
}

/**
 *
 * Given an 'associatedTuitionProfile' object, returns an array of 'tuitionFees'.
 *
 */
function unh_d1_client_getTuitionFees($associatedTuitionProfile = NULL) {
  $ret = array();
  if (!empty($associatedTuitionProfile) && array_key_exists('tuitionFees', $associatedTuitionProfile)) {
    $ret = _unh_d1_client_idxed_array($associatedTuitionProfile['tuitionFees']['tuitionFee']);
  }
  return $ret;
}


/**
 *
 * Given a 'tuitionFee' object, returns an array of 'tuitionFeeItems'.
 *
 */
function unh_d1_client_getTuitionFeeItems($tuitionFee = NULL) {
  $ret = array();
  if (!empty($tuitionFee) && array_key_exists('tuitionFeeItems', $tuitionFee)) {
    $ret = _unh_d1_client_idxed_array($tuitionFee['tuitionFeeItems']['tuitionFeeItem']);
  }
  return $ret;
}


/**
 *
 * Now, given the section, get the array of tuition items.
 *
 * We are running on the assumption that our case is really simple and has only
 * one interesting set of tuition items, which are contained in the first 
 * 'courseTuitionFees'.
 *
 * Returns a structure named 'tuition_info' containing:
 *  - an array of data structs, tuition:
 *    - dollar amount
 *    - discountable flag (true/false)
 *  - the published code
 *
 */
function unh_d1_client_getSectionTuitionInfo($section = array()) {
  $ret = array();
  
  if (!empty($section)) {
    $course_section_fees = unh_d1_client_getCourseSectionFees($section);
    if (!empty($course_section_fees)) {
      // Assuming only 1.
      $associated_section_fee_tuition_profiles = unh_d1_client_getAssociatedSectionFeeTuitionProfiles($course_section_fees[0]);
    
      $tuition_info = [];
      foreach($associated_section_fee_tuition_profiles as $profile) {
        $associated_tuition_profile = unh_d1_client_getAssociatedTuitionProfile($profile);
       
        // This is it!!!!!!
        if (!empty($associated_tuition_profile)) {
        
          $tuition_fees = unh_d1_client_getTuitionFees($associated_tuition_profile);
          
          foreach($tuition_fees as $tuition_fee) {
            $tuition_fee_items = unh_d1_client_getTuitionFeeItems($tuition_fee);
            
            $items = [];
            foreach($tuition_fee_items as $tuition_fee_item) {
              $item['amount'] = $tuition_fee_item['amount'];
              $item['discountable'] = $tuition_fee_item['discountable'];
              $items[] = $item;
            }
            $tuition['items'] = $items;
            $tuition['published_code'] = $tuition_fee['printCode'];
            $tuition_info[] = $tuition;
          }
           $ret = $tuition_info;
        }
        // This is it!!!!!!
        
      }  
    }
  }
  
  return $ret;
}


/******************************************************************************
 *
 * Functions to extract objectIds from SECTION objects obtained from the API.
 *
 ******************************************************************************/

/**
 *
 * Given an array of sections, returns an array of their 'objectId's.
 *
 */
function unh_d1_client_getSectionArrayObjectIds($sections = array()) {
  $ret = array();
  if (!empty($sections)) {
    $ret = array_column($sections, 'objectId');
  }
  return $ret;
}


/**
 *
 * Given a section, returns the 'associatedCourse' objectId.
 *
 */
function unh_d1_client_getSectionAssocCourseObjectId($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('associatedCourse', $section)) {
    $ret = $section['associatedCourse']['objectId'];
  }
  return $ret;
}



/******************************************************************************
 *
 * Functions to build various urls for action buttons.
 *
 * These urls should be module settings.
 *
 ******************************************************************************/
 
 //UTIL: Returns the enrollment url.
function unh_d1_client_get_enrollment_url($section) {
  // return 'https://learnforlife.unh.edu/search/publicCourseSearchDetails.do?method=addToCart&cspIndex=' . $sectionID;
  return 'https://' . unh_d1_client_settings_public_domain() . '/search/publicCourseSearchDetails.do?method=addToCart&cspIndex=' . unh_d1_client_getsectionObjectId($section);
}

//UTIL: Returns the wait list url.
function unh_d1_client_get_wait_list_url($section) {
  // return 'https://learnforlife.unh.edu/search/publicCourseSearchDetails.do?method=addToCart&cspIndex=' . $sectionID;
  return 'https://' . unh_d1_client_settings_public_domain() . '/search/publicCourseSearchDetails.do?method=addToCart&cspIndex=' . unh_d1_client_getsectionObjectId($section);
}

//UTIL: Returns the request more info url.
function unh_d1_client_get_request_more_info_url($section) {
  // return 'https://unhtestpv.destinysolutions.com/public/student/studentRequestInformation.do?method=edit&type=0&courseNumber=798519&notify=section'
  return 'https://' . unh_d1_client_settings_public_domain() . '/public/student/studentRequestInformation.do?method=edit&type=0&notify=section&courseNumber=' . unh_d1_client_getSectionAssocCourseObjectId($section);
}

/*
//UTIL: Returns the notify me url.  The actual url should be a module parameter.
function unh_d1_client_get_notify_me_url($courseObjID, $sectionNumber) {
  //https://unhtestpv.destinysolutions.com/public/student/studentRequestInformation.do
  //  ?method=edit&type=0&courseNumber=798628&sectionNumber=002&notify=course
  return 'https://' . variable_get('d1client_domain_public') . '/public/student/studentRequestInformation.do?method=edit&type=0&notify=course' .
    '&courseNumber=' . $courseObjID .
    '&sectionNumber=' . $sectionNumber;
}
*/
 

