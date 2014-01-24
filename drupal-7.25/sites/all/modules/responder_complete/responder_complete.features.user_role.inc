<?php
/**
 * @file
 * responder_complete.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function responder_complete_user_default_roles() {
  $roles = array();

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 2,
  );

  return $roles;
}
