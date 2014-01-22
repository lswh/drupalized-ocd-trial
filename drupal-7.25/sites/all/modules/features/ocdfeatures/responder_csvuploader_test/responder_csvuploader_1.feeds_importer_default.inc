<?php
/**
 * @file
 * responder_csvuploader_1.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function responder_csvuploader_1_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'responder';
  $feeds_importer->config = array(
    'name' => 'responder_test',
    'description' => 'CSV importer for responder data',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
        'directory' => 'public://feeds',
        'allowed_schemes' => array(
          0 => 'public',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => '1',
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'Name',
            'target' => 'field_responder_name',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'Src',
            'target' => 'field_source',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Lat',
            'target' => 'field_location:lat',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Long',
            'target' => 'field_location:lon',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'responder',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['responder'] = $feeds_importer;

  return $export;
}
