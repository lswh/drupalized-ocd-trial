<?php
/**
 * @file
 * responder_complete.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function responder_complete_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "openlayers" && $api == "openlayers_maps") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function responder_complete_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function responder_complete_node_info() {
  $items = array(
    'ol_usergroup' => array(
      'name' => t('OL_usergroup'),
      'base' => 'node_content',
      'description' => t('A local user group for open layer mapping'),
      'has_title' => '1',
      'title_label' => t('FieldProperty'),
      'help' => '',
    ),
    'responder' => array(
      'name' => t('responder'),
      'base' => 'node_content',
      'description' => t('This is the official entry form for the responder database'),
      'has_title' => '1',
      'title_label' => t('Responder Name'),
      'help' => t('Welcome, Regional OCD User! This is the responder entry form where you can add a new responder group in your region. Please fill out all the necessary fields accordingly.'),
    ),
  );
  return $items;
}
