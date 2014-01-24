<?php
/**
 * @file
 * responderform.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function responderform_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'Responder_Layer';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Responder_Layer';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Responder Locations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'openlayers_data';
  $handler->display->display_options['style_options']['data_source'] = array(
    'value' => 'wkt',
    'other_lat' => 'field_responder_name',
    'other_lon' => 'field_responder_name',
    'wkt' => 'field_location',
    'other_top' => 'field_responder_name',
    'other_right' => 'field_responder_name',
    'other_bottom' => 'field_responder_name',
    'other_left' => 'field_responder_name',
    'name_field' => 'field_responder_name',
    'description_field' => '#node',
    'view_mode' => 'teaser',
    'style_field' => '',
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_responder_name' => 'field_responder_name',
    'field_acronym' => 'field_acronym',
    'field_location' => 'field_location',
    'field_contact_person' => 'field_contact_person',
    'field_number_of_contact_person' => 'field_number_of_contact_person',
    'field_specialization_of_responde' => 'field_specialization_of_responde',
  );
  $handler->display->display_options['row_options']['separator'] = ',';
  /* Field: Content: Responder Name */
  $handler->display->display_options['fields']['field_responder_name']['id'] = 'field_responder_name';
  $handler->display->display_options['fields']['field_responder_name']['table'] = 'field_data_field_responder_name';
  $handler->display->display_options['fields']['field_responder_name']['field'] = 'field_responder_name';
  /* Field: Content: Acronym */
  $handler->display->display_options['fields']['field_acronym']['id'] = 'field_acronym';
  $handler->display->display_options['fields']['field_acronym']['table'] = 'field_data_field_acronym';
  $handler->display->display_options['fields']['field_acronym']['field'] = 'field_acronym';
  /* Field: Content: Location of Responder */
  $handler->display->display_options['fields']['field_location']['id'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['table'] = 'field_data_field_location';
  $handler->display->display_options['fields']['field_location']['field'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['label'] = '';
  $handler->display->display_options['fields']['field_location']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_location']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_location']['click_sort_column'] = 'geom';
  $handler->display->display_options['fields']['field_location']['settings'] = array(
    'data' => 'full',
  );
  /* Field: Content: Contact Person */
  $handler->display->display_options['fields']['field_contact_person']['id'] = 'field_contact_person';
  $handler->display->display_options['fields']['field_contact_person']['table'] = 'field_data_field_contact_person';
  $handler->display->display_options['fields']['field_contact_person']['field'] = 'field_contact_person';
  $handler->display->display_options['fields']['field_contact_person']['label'] = '';
  $handler->display->display_options['fields']['field_contact_person']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_contact_person']['element_label_colon'] = FALSE;
  /* Field: Content: Number of Contact Person */
  $handler->display->display_options['fields']['field_number_of_contact_person']['id'] = 'field_number_of_contact_person';
  $handler->display->display_options['fields']['field_number_of_contact_person']['table'] = 'field_data_field_number_of_contact_person';
  $handler->display->display_options['fields']['field_number_of_contact_person']['field'] = 'field_number_of_contact_person';
  $handler->display->display_options['fields']['field_number_of_contact_person']['label'] = '';
  $handler->display->display_options['fields']['field_number_of_contact_person']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_number_of_contact_person']['element_label_colon'] = FALSE;
  /* Field: Content: Specialization of Responder */
  $handler->display->display_options['fields']['field_specialization_of_responde']['id'] = 'field_specialization_of_responde';
  $handler->display->display_options['fields']['field_specialization_of_responde']['table'] = 'field_data_field_specialization_of_responde';
  $handler->display->display_options['fields']['field_specialization_of_responde']['field'] = 'field_specialization_of_responde';
  $handler->display->display_options['fields']['field_specialization_of_responde']['label'] = '';
  $handler->display->display_options['fields']['field_specialization_of_responde']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_specialization_of_responde']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'responder' => 'responder',
  );

  /* Display: OpenLayers Data Overlay */
  $handler = $view->new_display('openlayers', 'OpenLayers Data Overlay', 'openlayers_1');
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_responder_name' => 'field_responder_name',
    'field_acronym' => 'field_acronym',
    'field_location' => 'field_location',
    'field_contact_person' => 'field_contact_person',
    'field_number_of_contact_person' => 'field_number_of_contact_person',
    'field_specialization_of_responde' => 'field_specialization_of_responde',
  );
  $handler->display->display_options['row_options']['separator'] = ',';
  $export['Responder_Layer'] = $view;

  return $export;
}
