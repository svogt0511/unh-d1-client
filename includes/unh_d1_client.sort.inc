<?php

/**
 * @file
 *
 * @todo: Update this docblock.
 * @todo: make sure we get full output on other than 'short' informationLevel
 *
 * Functions mainly for sorting things.
 */

/**
 * Given a list of sections, returns them sorted by semester/(start)date/(section)title.
 * 
 */
function unh_d1_client_sort_sections_by_semesterStartDateSectionTitle($sections = array()) {
  $ret = array();

  if (!empty($sections)) {
    $semesters = unh_d1_client_getsemesters('full');
    
    if (!empty($semesters)) {
      // Now sort:
      //    first by semester (asc),
      //    then by start date (asc),
      //    then by section title (asc)
      $i = 0;
      $semester_objectIds = array_column($semesters, 'objectId');

      foreach ($sections as $section) {
        $key = array_search($section['associatedTerm']['objectId'], $semester_objectIds);
        if ($key === FALSE) {
          $key = 9999;
        }
        $sections[$i]['sort_semester'] = $key;
        $sections[$i]['sort_startDate'] = new DateTime($section['sectionStartDate']);
        $i++;
      }

      uasort($sections,
        function ($a, $b) {
          $ret = 0;
          if ($a['sort_semester'] < $b['sort_semester']) {
            $ret = -1;
          } elseif ($a['sort_semester'] > $b['sort_semester']) {
            $ret = 1;
          } else {
            if ($a['sort_startDate'] < $b['sort_startDate']) {
              $ret = -1;
            } elseif ($a['sort_startDate'] > $b['sort_startDate']) {
              $ret = 1;
            } else {
              $ret = strnatcasecmp($a['sectionTitle'], $b['sectionTitle']);
            }
          }
          return $ret;
        }
      );
      $ret = $sections;
    }
  }

  return $ret;
}


/**
 * Given a list of sections, returns them sorted by semester/(section)title.
 * 
 */
function unh_d1_client_sort_sections_by_semesterSectionTitle($sections = array()) {
  $ret = array();

  if (!empty($sections)) {
    $semesters = unh_d1_client_getsemesters('full');
    
    if (!empty($semesters)) {
      // Now sort:
      //    first by semester (asc),
      //    then by section title (asc)
      
      $i = 0;
      $semester_objectIds = array_column($semesters, 'objectId');
      
      foreach ($sections as $section) {
        $key = array_search($section['associatedTerm']['objectId'], $semester_objectIds);
        if ($key === FALSE) {
          $key = 9999;
        }
        $sections[$i]['sort_semester'] = $key;
        $i++;
      }

      uasort($sections,
        function ($a, $b) {
          $ret = 0;
          if ($a['sort_semester'] < $b['sort_semester']) {
            $ret = -1;
          } elseif ($a['sort_semester'] > $b['sort_semester']) {
            $ret = 1;
          } else {
            $ret = strnatcasecmp($a['sectionTitle'], $b['sectionTitle']);
          }
          return $ret;
        }
      );
      
      $ret = $sections;
    }
  }

  return $ret;
}


/**
 * Given a list of sections, returns them sorted by startDate/(section)title.
 * 
 */
function unh_d1_client_sort_sections_by_startDateSectionTitle($sections = array()) {
  $ret = array();

  if (!empty($sections)) {
      // Now sort:
      //    first by start date (asc),
      //    then by section title (asc)
      
      $i = 0;
      
      foreach ($sections as $section) {
        $sections[$i]['sort_startDate'] = new DateTime($section['sectionStartDate']);
        $i++;
      }

      uasort($sections,
        function ($a, $b) {
          $ret = 0;
          if ($a['sort_startDate'] < $b['sort_startDate']) {
            $ret = -1;
          } elseif ($a['sort_startDate'] > $b['sort_startDate']) {
            $ret = 1;
          } else {
            $ret = strnatcasecmp($a['sectionTitle'], $b['sectionTitle']);
          }
          return $ret;
        }
      );
      
      $ret = $sections;
  }

  return $ret;
}


/**
 * Given a list of sections, returns them sorted by (section)title.
 * 
 */
function unh_d1_client_sort_sections_by_sectionTitle($sections = array()) {
  $ret = array();

  if (!empty($sections)) {
      // Now sort:
      //    By section title (asc)

      uasort($sections,
        function ($a, $b) {
          return strnatcasecmp($a['sectionTitle'], $b['sectionTitle']);
        }
      );
      
      $ret = $sections;
  }

  return $ret;
}





