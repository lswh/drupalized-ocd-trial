<?php
/**
 * @file
 * ol.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function ol_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'comment-comment_node_ol_usergroup-comment_body'
  $field_instances['comment-comment_node_ol_usergroup-comment_body'] = array(
    'bundle' => 'comment_node_ol_usergroup',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'comment',
    'field_name' => 'comment_body',
    'label' => 'Comment',
    'required' => TRUE,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'node-ol_usergroup-field_location'
  $field_instances['node-ol_usergroup-field_location'] = array(
    'bundle' => 'ol_usergroup',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'geofield_map',
        'settings' => array(
          'geofield_map_baselayers_hybrid' => 1,
          'geofield_map_baselayers_map' => 1,
          'geofield_map_baselayers_physical' => 0,
          'geofield_map_baselayers_satellite' => 1,
          'geofield_map_controltype' => 'default',
          'geofield_map_draggable' => 0,
          'geofield_map_height' => '300px',
          'geofield_map_maptype' => 'map',
          'geofield_map_mtc' => 'standard',
          'geofield_map_overview' => 0,
          'geofield_map_overview_opened' => 0,
          'geofield_map_pancontrol' => 1,
          'geofield_map_scale' => 0,
          'geofield_map_scrollwheel' => 0,
          'geofield_map_streetview_show' => 0,
          'geofield_map_width' => '100%',
          'geofield_map_zoom' => 8,
        ),
        'type' => 'geofield_map_map',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_location',
    'label' => 'Location',
    'required' => FALSE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'openlayers',
      'settings' => array(),
      'type' => 'geofield_openlayers',
      'weight' => -3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Comment');
  t('Location');

  return $field_instances;
}
