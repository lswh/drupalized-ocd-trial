<?php
/**
 * @file
 * responder_csvuploader_1.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function responder_csvuploader_1_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}
