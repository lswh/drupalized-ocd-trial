<?php
/**
 * @file
 * ol.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function ol_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'ol_mapview';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'OL_mapview';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'OL_mapview';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'openlayers_map';
  $handler->display->display_options['style_options']['map'] = 'ol_display_of_sample_layer';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
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

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'ol-mapview';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Map';
  $handler->display->display_options['menu']['description'] = 'Map View of OCD Data';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['ol_mapview'] = $view;

  $view = new view();
  $view->name = 'ol_samplelayer';
  $view->description = 'Layer of sample data for the map';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'OL_samplelayer';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sample Data Type Layer';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1000';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'openlayers_data';
  $handler->display->display_options['style_options']['data_source'] = array(
    'value' => 'wkt',
    'other_lat' => 'field_location',
    'other_lon' => 'field_location',
    'wkt' => 'field_location',
    'other_top' => 'title',
    'other_right' => 'title',
    'other_bottom' => 'title',
    'other_left' => 'title',
    'name_field' => 'title',
    'description_field' => 'body',
    'view_mode' => 'full',
    'style_field' => '',
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Location of Responder */
  $handler->display->display_options['fields']['field_location']['id'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['table'] = 'field_data_field_location';
  $handler->display->display_options['fields']['field_location']['field'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_location']['click_sort_column'] = 'geom';
  $handler->display->display_options['fields']['field_location']['settings'] = array(
    'data' => 'full',
  );
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
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
    'ol_usergroup' => 'ol_usergroup',
  );

  /* Display: OpenLayers Data Overlay */
  $handler = $view->new_display('openlayers', 'OpenLayers Data Overlay', 'openlayers_1');
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $export['ol_samplelayer'] = $view;

  $view = new view();
  $view->name = 'ol_tabular';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'OL_Tabular';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'OL_Tabular';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Acronym */
  $handler->display->display_options['fields']['field_acronym']['id'] = 'field_acronym';
  $handler->display->display_options['fields']['field_acronym']['table'] = 'field_data_field_acronym';
  $handler->display->display_options['fields']['field_acronym']['field'] = 'field_acronym';
  /* Field: Content: Advanced SAR Equipment */
  $handler->display->display_options['fields']['field_advanced_sar_equipment']['id'] = 'field_advanced_sar_equipment';
  $handler->display->display_options['fields']['field_advanced_sar_equipment']['table'] = 'field_data_field_advanced_sar_equipment';
  $handler->display->display_options['fields']['field_advanced_sar_equipment']['field'] = 'field_advanced_sar_equipment';
  $handler->display->display_options['fields']['field_advanced_sar_equipment']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: Basic SAR Equipment */
  $handler->display->display_options['fields']['field_basic_sar_equipment']['id'] = 'field_basic_sar_equipment';
  $handler->display->display_options['fields']['field_basic_sar_equipment']['table'] = 'field_data_field_basic_sar_equipment';
  $handler->display->display_options['fields']['field_basic_sar_equipment']['field'] = 'field_basic_sar_equipment';
  $handler->display->display_options['fields']['field_basic_sar_equipment']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: City or Municipality */
  $handler->display->display_options['fields']['field_city_or_municipality']['id'] = 'field_city_or_municipality';
  $handler->display->display_options['fields']['field_city_or_municipality']['table'] = 'field_data_field_city_or_municipality';
  $handler->display->display_options['fields']['field_city_or_municipality']['field'] = 'field_city_or_municipality';
  /* Field: Content: Communication and Navigation */
  $handler->display->display_options['fields']['field_communication_and_navigati']['id'] = 'field_communication_and_navigati';
  $handler->display->display_options['fields']['field_communication_and_navigati']['table'] = 'field_data_field_communication_and_navigati';
  $handler->display->display_options['fields']['field_communication_and_navigati']['field'] = 'field_communication_and_navigati';
  $handler->display->display_options['fields']['field_communication_and_navigati']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: Contact Person */
  $handler->display->display_options['fields']['field_contact_person']['id'] = 'field_contact_person';
  $handler->display->display_options['fields']['field_contact_person']['table'] = 'field_data_field_contact_person';
  $handler->display->display_options['fields']['field_contact_person']['field'] = 'field_contact_person';
  /* Field: Content: Cooking Set */
  $handler->display->display_options['fields']['field_cooking_set']['id'] = 'field_cooking_set';
  $handler->display->display_options['fields']['field_cooking_set']['table'] = 'field_data_field_cooking_set';
  $handler->display->display_options['fields']['field_cooking_set']['field'] = 'field_cooking_set';
  $handler->display->display_options['fields']['field_cooking_set']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: Email */
  $handler->display->display_options['fields']['field_email']['id'] = 'field_email';
  $handler->display->display_options['fields']['field_email']['table'] = 'field_data_field_email';
  $handler->display->display_options['fields']['field_email']['field'] = 'field_email';
  /* Field: Content: Location of Responder */
  $handler->display->display_options['fields']['field_location']['id'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['table'] = 'field_data_field_location';
  $handler->display->display_options['fields']['field_location']['field'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['click_sort_column'] = 'geom';
  $handler->display->display_options['fields']['field_location']['settings'] = array(
    'data' => 'full',
  );
  /* Field: Content: Location of Responder */
  $handler->display->display_options['fields']['field_location_1']['id'] = 'field_location_1';
  $handler->display->display_options['fields']['field_location_1']['table'] = 'field_data_field_location';
  $handler->display->display_options['fields']['field_location_1']['field'] = 'field_location';
  $handler->display->display_options['fields']['field_location_1']['click_sort_column'] = 'geom';
  $handler->display->display_options['fields']['field_location_1']['settings'] = array(
    'data' => 'full',
  );
  /* Field: Content: Location of Responder (field_location) - proximity */
  $handler->display->display_options['fields']['field_geofield_distance']['id'] = 'field_geofield_distance';
  $handler->display->display_options['fields']['field_geofield_distance']['table'] = 'field_data_field_location';
  $handler->display->display_options['fields']['field_geofield_distance']['field'] = 'field_geofield_distance';
  $handler->display->display_options['fields']['field_geofield_distance']['precision'] = '0';
  $handler->display->display_options['fields']['field_geofield_distance']['geofield_proximity_manual'] = array(
    'lat' => '',
    'lon' => '',
  );
  $handler->display->display_options['fields']['field_geofield_distance']['geofield_proximity_entity_url_field'] = 'field_location';
  $handler->display->display_options['fields']['field_geofield_distance']['geofield_proximity_current_user_field'] = 'field_location';
  $handler->display->display_options['fields']['field_geofield_distance']['radius_of_earth'] = '6371';
  /* Field: Content: Manpower Number */
  $handler->display->display_options['fields']['field_manpower_number']['id'] = 'field_manpower_number';
  $handler->display->display_options['fields']['field_manpower_number']['table'] = 'field_data_field_manpower_number';
  $handler->display->display_options['fields']['field_manpower_number']['field'] = 'field_manpower_number';
  $handler->display->display_options['fields']['field_manpower_number']['settings'] = array(
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Number of Contact Person */
  $handler->display->display_options['fields']['field_number_of_contact_person']['id'] = 'field_number_of_contact_person';
  $handler->display->display_options['fields']['field_number_of_contact_person']['table'] = 'field_data_field_number_of_contact_person';
  $handler->display->display_options['fields']['field_number_of_contact_person']['field'] = 'field_number_of_contact_person';
  /* Field: Content: Other Equipment and Tools */
  $handler->display->display_options['fields']['field_other_equipment_and_tools']['id'] = 'field_other_equipment_and_tools';
  $handler->display->display_options['fields']['field_other_equipment_and_tools']['table'] = 'field_data_field_other_equipment_and_tools';
  $handler->display->display_options['fields']['field_other_equipment_and_tools']['field'] = 'field_other_equipment_and_tools';
  $handler->display->display_options['fields']['field_other_equipment_and_tools']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: Personal Protective Gear */
  $handler->display->display_options['fields']['field_personal_protective_gear']['id'] = 'field_personal_protective_gear';
  $handler->display->display_options['fields']['field_personal_protective_gear']['table'] = 'field_data_field_personal_protective_gear';
  $handler->display->display_options['fields']['field_personal_protective_gear']['field'] = 'field_personal_protective_gear';
  $handler->display->display_options['fields']['field_personal_protective_gear']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  $handler->display->display_options['fields']['field_personal_protective_gear']['delta_offset'] = '0';
  /* Field: Content: Personal Protective Gear (field_personal_protective_gear:delta) */
  $handler->display->display_options['fields']['delta']['id'] = 'delta';
  $handler->display->display_options['fields']['delta']['table'] = 'field_data_field_personal_protective_gear';
  $handler->display->display_options['fields']['delta']['field'] = 'delta';
  /* Field: Content: Power/Electric Equipment and Tools */
  $handler->display->display_options['fields']['field_power_electric_equipment_a']['id'] = 'field_power_electric_equipment_a';
  $handler->display->display_options['fields']['field_power_electric_equipment_a']['table'] = 'field_data_field_power_electric_equipment_a';
  $handler->display->display_options['fields']['field_power_electric_equipment_a']['field'] = 'field_power_electric_equipment_a';
  $handler->display->display_options['fields']['field_power_electric_equipment_a']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: Province */
  $handler->display->display_options['fields']['field_province']['id'] = 'field_province';
  $handler->display->display_options['fields']['field_province']['table'] = 'field_data_field_province';
  $handler->display->display_options['fields']['field_province']['field'] = 'field_province';
  /* Field: Content: Region */
  $handler->display->display_options['fields']['field_region']['id'] = 'field_region';
  $handler->display->display_options['fields']['field_region']['table'] = 'field_data_field_region';
  $handler->display->display_options['fields']['field_region']['field'] = 'field_region';
  /* Field: Content: Rescue Tools and Gadgets */
  $handler->display->display_options['fields']['field_rescue_tools_and_gadgets']['id'] = 'field_rescue_tools_and_gadgets';
  $handler->display->display_options['fields']['field_rescue_tools_and_gadgets']['table'] = 'field_data_field_rescue_tools_and_gadgets';
  $handler->display->display_options['fields']['field_rescue_tools_and_gadgets']['field'] = 'field_rescue_tools_and_gadgets';
  $handler->display->display_options['fields']['field_rescue_tools_and_gadgets']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: Responder Name */
  $handler->display->display_options['fields']['field_responder_name']['id'] = 'field_responder_name';
  $handler->display->display_options['fields']['field_responder_name']['table'] = 'field_data_field_responder_name';
  $handler->display->display_options['fields']['field_responder_name']['field'] = 'field_responder_name';
  /* Field: Content: Sleeping Gear */
  $handler->display->display_options['fields']['field_sleeping_gear']['id'] = 'field_sleeping_gear';
  $handler->display->display_options['fields']['field_sleeping_gear']['table'] = 'field_data_field_sleeping_gear';
  $handler->display->display_options['fields']['field_sleeping_gear']['field'] = 'field_sleeping_gear';
  $handler->display->display_options['fields']['field_sleeping_gear']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: Specialization of Responder */
  $handler->display->display_options['fields']['field_specialization_of_responde']['id'] = 'field_specialization_of_responde';
  $handler->display->display_options['fields']['field_specialization_of_responde']['table'] = 'field_data_field_specialization_of_responde';
  $handler->display->display_options['fields']['field_specialization_of_responde']['field'] = 'field_specialization_of_responde';
  /* Field: Content: Training Capability */
  $handler->display->display_options['fields']['field_training_capability']['id'] = 'field_training_capability';
  $handler->display->display_options['fields']['field_training_capability']['table'] = 'field_data_field_training_capability';
  $handler->display->display_options['fields']['field_training_capability']['field'] = 'field_training_capability';
  $handler->display->display_options['fields']['field_training_capability']['delta_offset'] = '0';
  /* Field: Content: Training Capability (field_training_capability:delta) */
  $handler->display->display_options['fields']['delta_1']['id'] = 'delta_1';
  $handler->display->display_options['fields']['delta_1']['table'] = 'field_data_field_training_capability';
  $handler->display->display_options['fields']['delta_1']['field'] = 'delta';
  /* Field: Content: Water Sanitation and Hygiene (WASH) */
  $handler->display->display_options['fields']['field_water_sanitation_and_hygie']['id'] = 'field_water_sanitation_and_hygie';
  $handler->display->display_options['fields']['field_water_sanitation_and_hygie']['table'] = 'field_data_field_water_sanitation_and_hygie';
  $handler->display->display_options['fields']['field_water_sanitation_and_hygie']['field'] = 'field_water_sanitation_and_hygie';
  $handler->display->display_options['fields']['field_water_sanitation_and_hygie']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: Water Search and Rescue Equipment (WASAR) */
  $handler->display->display_options['fields']['field_water_search_and_rescue_eq']['id'] = 'field_water_search_and_rescue_eq';
  $handler->display->display_options['fields']['field_water_search_and_rescue_eq']['table'] = 'field_data_field_water_search_and_rescue_eq';
  $handler->display->display_options['fields']['field_water_search_and_rescue_eq']['field'] = 'field_water_search_and_rescue_eq';
  $handler->display->display_options['fields']['field_water_search_and_rescue_eq']['settings'] = array(
    'edit' => 'Edit',
    'delete' => 'Delete',
    'add' => 'Add',
    'description' => 1,
    'view_mode' => 'full',
  );
  /* Field: Content: Website */
  $handler->display->display_options['fields']['field_website']['id'] = 'field_website';
  $handler->display->display_options['fields']['field_website']['table'] = 'field_data_field_website';
  $handler->display->display_options['fields']['field_website']['field'] = 'field_website';
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
    'ol_usergroup' => 'ol_usergroup',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'ol-tabular';
  $export['ol_tabular'] = $view;

  return $export;
}
