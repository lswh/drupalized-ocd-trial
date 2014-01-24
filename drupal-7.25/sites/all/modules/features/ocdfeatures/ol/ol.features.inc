<?php
/**
 * @file
 * ol.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ol_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "openlayers" && $api == "openlayers_layers") {
    return array("version" => "1");
  }
  if ($module == "openlayers" && $api == "openlayers_maps") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function ol_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function ol_node_info() {
  $items = array(
    'ol_usergroup' => array(
      'name' => t('OL_usergroup'),
      'base' => 'node_content',
      'description' => t('A local user group for open layer mapping'),
      'has_title' => '1',
      'title_label' => t('FieldProperty'),
      'help' => '',
    ),
  );
  return $items;
}
