<?php
/**
 * @file
 * responderform.features.inc
 */

/**
 * Implements hook_node_info().
 */
function responderform_node_info() {
  $items = array(
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
