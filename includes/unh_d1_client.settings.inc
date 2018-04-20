<?php

/**
 * @file
 *
 * @todo: Update this docblock.
 * @todo: make sure we get full output on other than 'short' informationLevel
 *
 * Functions/'methods' get settings data for this module.
 */

/******************************************************************************
 *
 * Calls to extract settings for this module.
 *
 ******************************************************************************/

function unh_d1_client_settings_public_domain() {
  $ret = NULL;
  
  $ret = variable_get('unh_d1_client_domain_public', '');
  
  return $ret;
}

function unh_d1_client_settings_fe_domain() {
  $ret = NULL;
  
  $ret = variable_get('unh_d1_client_domain_fe', '');
  
  return $ret;
}

function unh_d1_client_settings_be_domain() {
  $ret = NULL;
  
  $ret = variable_get('unh_d1_client_domain_be', '');
  
  return $ret;
}

function unh_d1_client_settings_api_release() {
  $ret = NULL;
  
  $ret = variable_get('unh_d1_client_api_release', '');
  
  return $ret;
}

function unh_d1_client_settings_cache_error_lifetime() {
  $ret = NULL;
  
  $ret = variable_get('unh_d1_client_cache_error_time', '');
  
  return $ret; 
}

function unh_d1_client_settings_cache_lifetime() {
  $ret = NULL;
  
  $ret = variable_get('unh_d1_client_cache_time', '');
  
  return $ret; 
}








