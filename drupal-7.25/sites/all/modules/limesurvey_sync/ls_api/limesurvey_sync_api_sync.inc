<?php
/**
 * @file
 * API functions for the ls_sync module
 */

/**
 * LimeSurvey Database class.
 *
 * This class provides an extension of the drupal database abstraction class.
 * The main purpose of this class is to access to a protected value : the
 * current active connection key. This class is not stored into the .module
 * file because it should be called only if necessary (not for other modules).
 */
class limesurvey_syncDatabase extends Database {
  /**
   * Returns current active connection key.
   *
   * Note that it is a public function.
   *
   * @return
   *   A string, most of the time : 'default' or 'ls'.
   */
  public function limesurvey_sync_active_db() {
    return self::$activeKey;
  }
}

/**
 * Return the active database connection name.
 *
 * Retrieve the active database connection name from the global variable
 * $active_db, using the db_set_active() Drupal function;
 *
 * @param $settings
 *   An array. Corresponding to the drupal variable ls_settings.
 *
 * @return
 *   A string, the active database name
 */
function limesurvey_sync_active_db_name() {
  // Note : the Database::$activeKey provide the current active database key
  // but this is a protected value from the Database class. We create a child
  // class, for retrieving its value from a public function.
  $lms = new limesurvey_syncDatabase();
  return $lms->limesurvey_sync_active_db();
}

/**
 * Activate the LimeSurvey database.
 *
 * Function called before each query or group of querys on the Limesurvey
 * database.
 *
 * @return
 *   A string, the name of the previously active database, or FALSE if none
 *   was found.
 */
function limesurvey_sync_db_open() {
  $current_db = limesurvey_sync_active_db_name();
  // Don't call the db_set_active() function if it not necessary.
  if ($current_db != 'ls') {
    return db_set_active('ls');
  }
  return 'ls';
}

/**
 * Desactivate the LimeSurvey database.
 *
 * Function called after each query or group of querys on the Limesurvey
 * database, to allow the system to switch back to the Drupal database or keep
 * staying on the LimeSurvey database depending on the previously active
 * database.
 *
 * @param $db_to_active
 *   A string. the name of the database connection to active on closing the
 *   LimeSurvey database connection. Default is 'default', corresponding to the
 *   Drupal system database.
 *
 * @return
 *   A string, the name of the current active database, after the
 *   limesurvey_sync_db_close() is called.
 */
function limesurvey_sync_db_close($db_to_active = 'default') {
  // Don't call the db_set_active() function if it not necessary.
  $current_db = limesurvey_sync_active_db_name();
  if ($current_db != $db_to_active) {
    db_set_active($db_to_active);
  }
  return $db_to_active;
}

/**
 * Provide the LimeSurvey database table name.
 *
 * Depending on the selected table, concatenate the LimeSurvey database table
 * name to the survey ID. For example,
 * limesurvey_sync_table('surveys', 12356) returns 'surveys_123456'.
 *
 * @param $table
 *   A string. Corresponding to the LimeSurvey table generic name. Default is
 *   'survey'.
 * @param $sid
 *   (optional) An integer. Corresponding to the survey ID reference. Default
 *   is empty.
 *
 * @return
 *   A string, corresponding to the LimeSurvey database name.
 */
function limesurvey_sync_table($table = 'surveys', $sid = '') {
  switch ($table) {
    // Handle tokens and answers tables : they need the $sid parameter.
    case 'tokens':
    case 'survey':
    case 'answer':
      // Watch out ! The answerS table exists to in the Limesurvey schema !!
      if ($table == 'answer') {
        // Answer is an more explicit name for the survey answers table name.
        $table = 'survey';
      }
      $table_name = $table . '_' . (string) $sid;
      break;
    default:
      $table_name = $table;
  }
  return check_plain($table_name);
}


/**
 * Provide The LimeSurvey global settings.
 *
 * Two LimeSurvey global settings are used into the LimeSurvey Sync module :
 * 'timeadjust' for the Timezone (and date) features and 'DBVersion' for
 * handling various bugs or features depending on the LimeSurey build version.
 *
 * @return
 *   An array, the Limesurvey global settings values keyed fy there name, or
 *   FALSE if the settings_global page does not exist.
 */
function limesurvey_sync_ls_global_settings() {
  static $ls_gs = array();
  if (empty($ls_gs)) {
    // Set the setting names to retrieve with their default values.
    $settings_to_store = array(
      'timeadjust' => '+0 hours',
      'DBVersion' => 0,
    );
    $previous_db = limesurvey_sync_db_open();
    $table_exists = db_table_exists('settings_global');
    limesurvey_sync_db_close($previous_db);
    if ($table_exists) {
      // Retrieve the limeSurvey global settings values.
      limesurvey_sync_db_open();
      $gs_query = db_query('SELECT stg_name, stg_value FROM {' . limesurvey_sync_table('settings_global') . '}');
      while ($setting = $gs_query->fetchAssoc()) {
        if (array_key_exists($setting['stg_name'], $settings_to_store)) {
          $ls_gs[$setting['stg_name']] = $setting['stg_value'];
        }
      }
      limesurvey_sync_db_close($previous_db);
      // Make sure the keys exist (avoid to return an error if the table does
      // not exist).
      foreach ($settings_to_store as $name => $default_value) {
        if (!isset($ls_gs[$name])) {
          $ls_gs[$name] = $default_value;
        }
      }
    }
    else {
      return FALSE;
    }
  }
  return $ls_gs;
}

/**
 * Fix for the LimeSurvey storing date system.
 *
 * The LimeSurvey software stores datetime values including the timezone value
 * whereas the Drupal system stores timestamp value and apply then the timezone
 * parameter. This function returns the correct datetime value applying the
 * LimeSurvey timezone from a timestamp. As long as the Limesurvey site and the
 * Drupal site are using the same timezone (this is checked by the
 * synchronization setting page), there is no adaptations.
 *
 * @param $timestamp
 *   A integer. The timestamp to be convert to the LimeSurvey datetime system,
 *   default is NULL corresponding to the current time.
 *
 * @return
 *   A string, the english formated corresponding date.
 */
function limesurvey_sync_fixdate_drupal_to_ls($timestamp = NULL) {
  if ($timestamp == NULL) {
    $timestamp = REQUEST_TIME;
  }
  $delta_time = variable_get('ls_settings', array('ls_delta_time' => 0));
  $drupal_timezone = variable_get('date_default_timezone', @date_default_timezone_get());
  // The date() php function uses the user timezone in order to
  // determine the timezone.
  $user_timezone = drupal_get_user_timezone();
  module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
  $delta_tz = limesurvey_sync_get_timezone_offset($drupal_timezone, $user_timezone);
  return date('Y-m-d H:i:s', ($timestamp + $delta_tz + $delta_time['ls_delta_time']));
}

/**
 * Convert english date to the synchronized timestamp.
 *
 * As an example, convert '2012-01-01 10:22:00' to 1325409720, adjusting it if
 * Drupal and LimeSurvey servers are not synchronized or use a specific
 * timezone.
 *
 * @param $english_date
 *   A string. The english formated date.
 *
 * @return
 *   A numeric. The Drupal timestamp corresponding value.
 */
function limesurvey_sync_fixdate_ls_to_drupal($english_date) {
  if (empty($english_date)) {
    return '';
  }
  $delta_time = variable_get('ls_settings', array('ls_delta_time' => 0));
  // Ignore PHP strict notice if time zone has not yet been set in the php.ini
  // configuration.
  $drupal_timezone_name = variable_get('date_default_timezone', @date_default_timezone_get());
  // Assume that the LimeSurvey timezone is the same as the Drupal timezone.
  return (strtotime($english_date . ' ' . $drupal_timezone_name) - $delta_time['ls_delta_time']);
}

/**
 * Append the timezone to a LimeSurvey english date.
 *
 * This function is used to display non dynamic date that won't be adapted to
 * the user timezone. For instance : the submidate, or the startdate stored
 * into an html table. '2012-01-01 10:22:00' may returns
 * '2012-01-01 10:22:00 Europe/Paris'.
 *
 * @param $english_date
 *   A string. The LimeSurvey english formated date.
 *
 * @return
 *   A translated date string in the requested format.
 */
function limesurvey_sync_format_static_lsdate($english_date) {
  if (empty($english_date)) {
    return t('No date');
  }
  $timestamp = limesurvey_sync_fixdate_ls_to_drupal($english_date);
  $drupal_timezone_name = variable_get('date_default_timezone', @date_default_timezone_get());
  // Don't append the timezone for sites that don't care about timezone.
  $append_tz = (module_exists('locale')) ? ' ' . $drupal_timezone_name : '';
  return format_date($timestamp, 'short', '', $drupal_timezone_name) . $append_tz;
}

/**
 * Provide the LimeSurvey major version.
 *
 * The LimeSurvey version is retrieved by the DBVersion value stored into the
 * database. I don't use the value from the version.php file because it may be
 * not reachable if the LimeSurvey software is stored on an external server.
 * Note :
 * - the 1.92+ version build 120919 owns the 155.6 DBVersion,
 * - the v2.0+ build 121019 owns the 163 DBVersion.
 * I arbitrary assume that the LimeSurvey v2.0 have started from the 160
 * DBVersion.
 *
 * @return
 *   A string, '1.x' or '2.x', or FALSE if it fails.
 */
function limesurvey_sync_lsversion() {
  $ls_v2_dbversion = 160;
  static $lsversion = NULL;
  // Use cache.
  if ($lsversion ===  NULL) {
    $ls_gs = limesurvey_sync_ls_global_settings();
    if (!empty($ls_gs['DBVersion'])) {
      $lsversion = ($ls_gs['DBVersion'] < $ls_v2_dbversion) ? '1.x' : '2.x';
    }
    else {
      // If there are some synchronization troubles, act as if it is the
      // LimeSurvey v2.x version (the actual stable version).
      $lsversion = '2.x';
    }
  }
  return $lsversion;
}

/**
 * Provide url to specific pages of the LimeSurvey admin interface.
 *
 * This function is safe, sanitize values that may be displayed with no more
 * control (via form_set_error() as an example).
 *
 * @param $sid
 *   A numeric. Corresponding to the survey reference ID. Default is NULL.
 * @param $page
 *   A string. The page. Check the function for available values. Default is
 *   'admin'.
 * @param $anchor
 *   A string. The anchor value. Default is empty.
 * @param $absolute
 *   A boolean. For absolute url. Default is TRUE.
 * @param $aid
 *   an numeric string, the LimeSurvey answer ID. Default is empty
 *
 * @return
 *   A string, The corresponding url.
 */
function limesurvey_sync_admin_url($sid = NULL, $page = 'admin', $anchor = '', $absolute = TRUE, $aid = '') {
  $options = array(
    'absolute' => $absolute,
    'external' => TRUE,
  );

  $ls_v2 = (limesurvey_sync_lsversion() != '1.x');
  $admin_page = ($ls_v2) ? '/index.php/admin/' : '/admin/admin.php';
  $ls_settings = variable_get('ls_settings', array('ls_path' => ''));
  $url = check_plain($ls_settings['ls_path'] . $admin_page);
  if ($absolute) {
    module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
    $url = limesurvey_sync_rel2abs($url);
  }

  switch ($page) {
    case 'globalsettings':
      if ($ls_v2) {
        $url .= 'globalsettings';
      }
      else {
        $options['query']['action'] = 'globalsettings';
      }
      break;
    case 'newsurvey':
      if ($ls_v2) {
        $url .= 'survey/newsurvey';
      }
      else {
        $options['query']['action'] = 'newsurvey';
      }
      break;
    case 'listsurveys':
      if ($ls_v2) {
        $url .= 'survey/index';
      }
      else {
        $options['query']['action'] = 'listsurveys';
      }
      break;
      // Below : need survey ID.
    case 'admin':
      if ($ls_v2) {
        $url .= 'survey/view';
      }
      break;
    case 'editsurveysettings':
      if ($ls_v2) {
        $url .= 'survey/editsurveysettings';
      }
      else {
        $options['query']['action'] = 'editsurveysettings';
      }
      break;
    case 'activate':
      if ($ls_v2) {
        $url .= 'survey/activate';
      }
      else {
        $options['query']['action'] = 'activate';
      }
      break;
    case 'statistics':
      if ($ls_v2) {
        $url .= 'statistics/index';
      }
      else {
        $options['query']['action'] = 'statistics';
      }
      break;
    case 'tokens':
      if ($ls_v2) {
        $url .= 'tokens/index';
      }
      else {
        $options['query']['action'] = 'tokens';
      }
      break;
    case 'browse':
      if ($ls_v2) {
        $url .= 'responses/index';
      }
      else {
        $options['query']['action'] = 'browse';
      }
      break;
    case 'editsurveylocalesettings':
      if ($ls_v2) {
        $url .= 'survey/editlocalsettings';
      }
      else {
        $options['query']['action'] = 'editsurveylocalesettings';
      }
      break;
    case 'responseview':
      if ($ls_v2) {
        $url .= 'responses/view/';
      }
      else {
        $options['query']['action'] = 'browse';
      }
      break;
  }

  switch ($anchor) {
    case 'presentation':
      $options['fragment'] = 'presentation';
      break;
    case 'tokens':
      $options['fragment'] = 'tokens';
      break;
    case 'general':
      $options['fragment'] = 'general';
      break;
    case 'publication':
      $options['fragment'] = 'publication';
      break;
    case 'notification':
      $options['fragment'] = 'notification';
      break;
    case 'copy':
      $options['fragment'] = 'copy';
      break;
  }
  if (preg_match('#^edittxtele[0-9]{1,2}$#', $anchor) || preg_match('#^url_[a-zA-Z\-]{0,12}$#', $anchor)) {
    // Handle : 'edittxtele0' stands for default language, 'edittxtele1' for
    // the first additional language, etc... And the 'url_en', url_fr' for
    // targeting the end url textfield; thanks to the
    // limesurvey_sync_localsettings_anchor() function.
    $options['fragment'] = $anchor;
  }

  $goto_default_page = FALSE;
  switch ($page) {
    case 'admin':
    case 'editsurveysettings':
    case 'activate':
    case 'statistics':
    case 'tokens':
    case 'browse':
    case 'editsurveylocalesettings':
    case 'responseview':
      if (!empty($sid)) {
        if ($ls_v2) {
          $url .= '/surveyid/' . $sid;
        }
        else {
          $options['query']['sid'] =  $sid;
        }
        if ($page == 'responseview') {
          if (!empty($aid) && is_numeric($aid) && $aid > 0) {
            if ($ls_v2) {
              $url .= '/id/' . $aid;
            }
            else {
              $options['query']['id'] =  $aid;
            }
          }
          else {
            $goto_default_page = TRUE;
          }
        }
      }
      else {
        $goto_default_page = TRUE;
      }
      break;
    default:
  }
  if ($goto_default_page) {
    // If sid id not provide, those pages do not exist.
    // Go to the defaut admin page instead.
    if ($ls_v2) {
      $url = $ls_settings['ls_path'] . $admin_page . 'survey/index';
    }
    else {
      unset($options['query']['action'], $options['fragment']);
    }
  }

  // Set 'sid' before 'action' in the url.
  if (isset($options['query']) && is_array($options['query'])) {
    krsort($options['query']);
  }

  return url($url, $options);
}

/**
 * Create or delete a fake empty survey.
 *
 * It is used to check if the Limesurvey database connexion on drupal matchs
 * the same LimeSurvey site via url. It is usefulled when the server stores
 * several LimeSurvey installations. This function creates a fake survey with
 * a specific name, then the Drupal site try to reach this specific survey. At
 * last, the function delete this survey.
 *
 * @param $sid
 *   A numeric. A fake survey ID value (between 100 and 999). Default is empty,
 *   the survey ID will be randomly generate.
 * @param $action
 *   A string. Values are 'create' or 'delete' for creating or deleteing the
 *   survey. Default is 'create'.
 *
 * @return
 *   A numeric, the fake Survey ID if the creation or deletion succeed or FALSE
 *   if it fails.
 */
function limesurvey_sync_fake_survey($sid = '', $action = 'create') {
  if (!in_array($action, array('create', 'delete'))) {
    return FALSE;
  }
  // Set the previous db.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $previous_db = limesurvey_sync_db_open();
  $success1 = $success2 = FALSE;
  if ($action == 'create') {
    // First create a unique SID. Standard SID is 5 digits for LSv1.x and 6
    // digits for LSv2.x. Use 3 digits.
    if (empty($sid)) {
      $sid = rand(100, 999);
    }
    // Create a fake survey with a name, 'listpublic' and 'active' to 'Y'
    // (default are 'N').
    $for_fake_survey = array(
      'sid' => $sid,
      'active' => 'Y',
      'listpublic' => 'Y',
    );
    // Fix colomns with no default value :
    $fix_insert = array(
      'owner_id' => 1,
      'admin' => 'The LimeSurvey Sync module',
      'language' => 'en',
      'datecreated' => limesurvey_sync_fixdate_drupal_to_ls(),
    );
    db_insert(limesurvey_sync_table('surveys'))
      ->fields(array_merge($for_fake_survey, $fix_insert))
      ->execute();
    db_insert(limesurvey_sync_table('surveys_languagesettings'))
      ->fields(array(
      'surveyls_survey_id' => $sid,
      'surveyls_title' => format_string('Fake survey @sid from the drupal site @url', array('@sid' => '#' . ((string) $sid), '@url' => url(NULL, array('absolute' => TRUE)))),
      'surveyls_description' => format_string('This survey is not functionnal, it is generated by the LimeSurvey Sync Drupal module from the synchronisation setting page @url', array('@url' => url('admin/config/media/ls_sync', array('absolute' => TRUE)))),
    ))
      ->execute();
    // There is no schema database so no declared primary key : the returned
    // value from db_insert() is untrusted.
    $success1 = $success2 = TRUE;
  }
  else { // $action == 'delete')
    if (!empty($sid)) {
      $success1 = db_delete(limesurvey_sync_table('surveys'))
        ->condition('sid', $sid)
        ->execute();
      $success2 = db_delete(limesurvey_sync_table('surveys_languagesettings'))
        ->condition('surveyls_survey_id', $sid)
        ->execute();
    }
  }
  limesurvey_sync_db_close($previous_db);
  return ($success1 && $success2) ? ((string) $sid) : FALSE;
}
