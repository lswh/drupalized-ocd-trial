<?php
/**
 * @file
 * responderform.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function responderform_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer field collections'.
  $permissions['administer field collections'] = array(
    'name' => 'administer field collections',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_collection',
  );

  // Exported permission: 'create responder content'.
  $permissions['create responder content'] = array(
    'name' => 'create responder content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any responder content'.
  $permissions['delete any responder content'] = array(
    'name' => 'delete any responder content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own responder content'.
  $permissions['delete own responder content'] = array(
    'name' => 'delete own responder content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any responder content'.
  $permissions['edit any responder content'] = array(
    'name' => 'edit any responder content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own responder content'.
  $permissions['edit own responder content'] = array(
    'name' => 'edit own responder content',
    'roles' => array(),
    'module' => 'node',
  );

  return $permissions;
}
