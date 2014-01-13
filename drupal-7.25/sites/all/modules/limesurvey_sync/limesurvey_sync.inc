<?php
/**
 * @file
 * More functions for the limesurvey_sync module.
 */

/**
 * Provide link to specific pages of the LimeSurvey admin interface.
 *
 * @param $text
 *   An string. The link text.
 * @param $sid
 *   An numeric. Corresponding to the survey reference ID. Default is NULL.
 * @param $page
 *   A string. The page. Check the function for available values. Default is
 *   'admin'.
 * @param $anchor
 *   An string. The anchor value. Default is empty.
 * @param $absolute
 *   A boolean. For absolute url. Default is TRUE.
 * @param $new_windows
 *   A boolean. Open the LimeSurvey admin interface into a new window. Default
 *   is TRUE.
 * @param $aid
 *   an numeric string, the LimeSurvey answer ID. Default is empty
 *
 * @return
 *   A string, The corresponding url.
 */
function limesurvey_sync_link_to_ls($text, $sid = NULL, $page = 'admin', $anchor = '', $absolute = TRUE, $new_windows = TRUE, $aid = '') {
  $windows_tag = array(
    'attributes' => array(
      'title' => t('Open the LimeSurvey admin interface in a new window'),
      'onclick' => 'window.open(this.href); return false;',
    ),
  );
  $windows = ($new_windows) ? $windows_tag : array();
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $url = limesurvey_sync_admin_url($sid, $page, $anchor, $absolute, $aid);
  return l($text, $url, $windows);
}

/**
 * Watchdog changes on array variables.
 *
 * This function watchdogs changes on array variables, expliciting changes. It
 * does not handle variable with numeric keys but it handles one level of
 * multidimensional arrays.
 *
 * @param $variable
 *   An string. Corresponding to the variable name.
 * @param $old_settings
 *   An array. Corresponding to variable values before changes.
 * @param $new_settings
 *   An array. Corresponding to variable values after changes (actual).
 * @param $module
 *   An string. Corresponding to the module name responsible for changes.
 * @param $severity
 *   An constant. Corresponding to the watchdog severity. Default to
 *   WATCHDOG_NOTICE.
 */
function limesurvey_sync_custom_watchdog($variable, $old_settings, $new_settings, $module = 'limesurvey_sync', $severity = WATCHDOG_NOTICE) {

  if ($new_settings != $old_settings) {
    $all_keys = array_merge($old_settings, $new_settings);
    foreach ($all_keys as $key => $change) {
      $values = array(
        '%variable' => $variable,
        '@field' => $key,
      );
      if (isset($new_settings[$key]) && isset($old_settings[$key])) {
        if ($old_settings[$key] != $new_settings[$key]) {
          $values['%old_val'] = (!is_array($old_settings[$key])) ? $old_settings[$key] : '(' . implode(', ', $old_settings[$key]) . ')';
          $values['%new_val'] = (!is_array($new_settings[$key])) ? $new_settings[$key] : '(' . implode(', ', $new_settings[$key]) . ')';
          // The first two watchdog() parameters should be literal strings.
          if ($module == 'limesurvey_sync') {
            watchdog('limesurvey_sync', 'The %variable settings have changed : @field from %old_val to %new_val', $values, $severity);
          }
          elseif ($module == 'ls_survey') {
            watchdog('ls_survey', 'The %variable settings have changed : @field from %old_val to %new_val', $values, $severity);
          }
        }
      }
      elseif (!isset($old_settings[$key]) && isset($new_settings[$key])) {
        $values['%val'] = (!is_array($new_settings[$key])) ? $new_settings[$key] : '(' . implode(', ', $new_settings[$key]) . ')';
        // The first two watchdog() parameters should be literal strings.
        if ($module == 'limesurvey_sync') {
          watchdog('limesurvey_sync', 'The %variable settings have changed : @field as been added (value is %val)', $values, $severity);
        }
        elseif ($module == 'ls_survey') {
          watchdog('ls_survey', 'The %variable settings have changed : @field as been added (value is %val)', $values, $severity);
        }
      }
      elseif (isset($old_settings[$key]) && !isset($new_settings[$key])) {
        $values['%val'] = (!is_array($old_settings[$key])) ? $old_settings[$key] : '(' . implode(', ', $old_settings[$key]) . ')';
        // The first two watchdog() parameters should be literal strings.
        if ($module == 'limesurvey_sync') {
          watchdog('limesurvey_sync', 'The %variable settings have changed : @field as been deleted (value was %val)', $values, $severity);
        }
        elseif ($module == 'ls_survey') {
          watchdog('ls_survey', 'The %variable settings have changed : @field as been deleted (value was %val)', $values, $severity);
        }
      }
    }
  }
}

/**
 * Return if the LimeSurvey and the Drupal sites are sharing the same storage system.
 *
 * Inform if the LimeSurvey and the Drupal sites are using the same server or
 * the database.
 *
 * @return
 *   An array, values are the not shared properties, availables values are
 *   'driver', 'username', 'host', 'database', 'prefix'. Or False if the
 *   LimeSurvey database connection is not declared into the setting.php, or
 *   NULL if they share the same properties.
 */
function limesurvey_sync_distinct_storage() {
  // Test if The LimeSurvey hosting server is the same that the drupal
  // hosting server.
  global $databases;
  if (!isset($databases['ls']['default']) || !is_array($databases['ls']['default'])) {
    // The LimeSurvey database connection is not declared into the setting.php
    // file.
    return FALSE;
  }

  // Copy the $database variable, keeping only the server informations.
  $server_keys = array(
    'driver' => '',
    'database' => '',
    'host' => '',
    'username' => '',
    'prefix' => '',
  );
  $bd_drupal = array_map('drupal_strtolower', array_intersect_key($databases['default']['default'], $server_keys));
  $bd_ls = array_map('drupal_strtolower', array_intersect_key($databases['ls']['default'], $server_keys));
  $diff = array_keys(array_diff_assoc($bd_ls, $bd_drupal));
  return (!empty($diff)) ? $diff : NULL;
}

/**
 * Calculate time difference between the Drupal server ad the LimeSurvey server.
 *
 * Both servers may not use the same time. This function calculates the
 * difference in seconds.
 *
 * @return
 *   A sight numeric. The time difference in seconds or FALSE if the driver is
 *   not mysql, mysqli, pgsql or sqlite, or an error occurs.
 */
function limesurvey_sync_calculate_server_time_diff() {
  // Test if The LimeSurvey hosting server is the same that the drupal
  // hosting server.
  $distinct_storage = limesurvey_sync_distinct_storage();

  if (!$distinct_storage) {
    return FALSE;
  }
  if (!in_array('host', $distinct_storage)) {
    // The most simple case, Drupal and Limesurvey use the same servor, time
    // should be the same.
    $delta_time = 0;
  }
  else {
    $servers = array('default', 'ls');
    global $databases;
    // Load file for the limesurvey_sync functions.
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
    foreach ($servers as $server) {
      if ($server == 'ls') {
        $previous_db = limesurvey_sync_db_open();
      }
      switch (drupal_strtolower($databases[$server]['default']['driver'])) {
        case 'mysqli':
        case 'mysql':
          $asking_for_the_date = 'SELECT UNIX_TIMESTAMP()';
          break;
        case 'pgsql':
          $asking_for_the_date = "SELECT date_part('epoch',now())";
          break;
        case 'sqlite':
          $asking_for_the_date = "SELECT strftime('%s','now')";
          break;
        default:
          return FALSE;
      }
      $date_on_server[$server] = db_query($asking_for_the_date)->fetchField();
      if ($server == 'ls') {
        limesurvey_sync_db_close($previous_db);
      }
    }

    if ($date_on_server['ls'] != FALSE) {
      $delta_time = $date_on_server['ls'] - $date_on_server['default'];
    }
    else {
      $delta_time = 0;
    }
  }
  return $delta_time;
}

/**
 * Test if the LimeSurvey synchronization settings are valid.
 *
 * This function tests if the LimeSurvey synchronization settings are valid
 * and displays results into a multimensional array. It checks the connection
 * to the LimeSurvey database, if the LimeSurvey tables exist on the database,
 * the LimeSurvey website connection via http and the time difference between
 * both servers.
 *
 * @param $settings
 *   An array. Corresponding to the drupal variable ls_settings.
 *
 * @return
 *   A multimensional array providing results : Primary keys are 'server',
 *   'table', 'url', 'time' and, on each one, secondary keys are 'title' (a
 *   string, the property tested), 'result' (a boolean, TRUE if the test has
 *   succeeded), 'message' (a string, message to display), and 'help' : a
 *   string, the help text).
 */
function limesurvey_sync_test_interface($settings) {
  $test_result = array();
  $windows_tag = array(
    'attributes' => array(
      'title' => t('Open the link into a new window'),
      'onclick' => 'window.open(this.href); return false;',
    ),
  );

  // Store some distinct datas from LimeSurvey 1.x and 2.x.
  $configfile_1x = 'config.php';
  $configfile_2x = 'application/config/config.php';
  $dbprefix_1x = '$dbprefix';
  $dbprefix_2x = 'tablePrefix';
  $admin_login_page_1x = '/admin/admin.php';
  $admin_login_page_2x = '/index.php/admin/authentication/login';
  $fake_survey_prefix = '/index.php';

  // Load file for the limesurvey_sync_lsversion() function.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  if (limesurvey_sync_ls_global_settings()) {
    $configfile = (limesurvey_sync_lsversion() != '1.x') ? $configfile_2x : $configfile_1x;
    $dbprefix = (limesurvey_sync_lsversion() != '1.x') ? $dbprefix_2x : $dbprefix_1x;
    $replacing_string = t('replacing each field by the correct value from the LimeSurvey %lsconfig file.', array('%lsconfig' => $configfile));
    $find_it_string =  t('you can find it opening the %lsconfig file stored into your LimeSurvey site folder (it is the %dbprefix variable value).', array('%lsconfig' => $configfile, '%dbprefix' => $dbprefix));
  }
  else {
    // The connexion fails, we can not determine if it is an LimeSurvey 1.x or
    // 2.x install.
    $replacing_string = t('replacing each field by the correct value from the LimeSurvey %configfile_2x file (LimeSurvey v2.x) or the %configfile_1x file (LimeSurvey v1.x).', array('%configfile_1x' => $configfile_1x, '%configfile_2x' => $configfile_2x));
    $find_it_string =  t('you can find it : under LimeSurvey v2.x opening the %lsconfig_2x file stored into your LimeSurvey site folder (it is the %dbprefix_2x variable value) or under LimeSurvey v1.x opening the %lsconfig_1x file (it is the %dbprefix_1x variable value).', array('%lsconfig_1x' => $configfile_1x, '%lsconfig_2x' => $configfile_2x, '%dbprefix_1x' => $dbprefix_1x, '%dbprefix_2x' => $dbprefix_2x));
  }

  // Test if The LimeSurvey hosting server has been configured.
  // Set message for some other tests.
  global $databases;
  $server_fails_comment = t('The LimeSurvey site hosting server test must succeed in order to run this test.');
  $server_fails_help = t('You need to fix the test of the LimeSurvey site hosting server first.');
  $distinct_storage = limesurvey_sync_distinct_storage();
  if ($distinct_storage === FALSE) {
    $server_ok = FALSE;
    $server_output =  '<strong>' . t('The LimeSurvey site hosting server is not correctly declared.') . '</strong>';
  }
  else {
    if (!in_array('host', $distinct_storage)) {
      // The database, username, or db_type may be different.
      $database_string = (in_array('database', $distinct_storage)) ? '<br />' . t('and are using distinct databases') : '';
      $invalid_prefix_string = (!in_array('database', $distinct_storage) && !in_array('prefix', $distinct_storage)) ? '<br /><strong>' . t('but the LimeSurvey database prefix is not valid, choose a value other than %invalid.', array('%invalid' => ((!empty($databases['ls']['default']['prefix'])) ? $databases['ls']['default']['prefix'] :  t('empty')))) . '</strong>' : '';

      $server_output =  t('The LimeSurvey and the Drupal sites are sharing the same hosting server!database!prefix.', array('!database' => $database_string, '!prefix' => $invalid_prefix_string));
    }
    else {
      $server_output =  t('The LimeSurvey and the Drupal sites are not sharing the same hosting server.');
    }

    if (db_set_active('ls')) {
      // If connection was succesfull, switch back to the previous server (the
      // Drupal one).
      db_set_active('default');
      $server_ok = TRUE;
      $server_output .= '<br />' . t('The database connection has succeeded.');
    }
    else {
      $server_ok = FALSE;
      $server_output .= '<br /><strong>' . t('The database connection has failed.') . '</strong>';
    }

  }

  $server_list = array();
  $server_list[] = '<strong>' . t("The LimeSurvey database connection informations must be appended into the Drupal file <em>sites/default/settings.php</em>, adding the lines below after the \$databases['default']['default'] variable :") . '</strong>'
  . "\n\t\t" . '<br />' . '<blockquote>'
  . "\n\t\t" . "\$databases<strong>['ls']</strong>['default'] = array("
  . "\n\t\t" . '<br />' . "'driver' => 'your_LimeSurvey_driver',"
  . "\n\t\t" . '<br />' . "'database' => 'your_LimeSurvey_database_name',"
  . "\n\t\t" . '<br />' . "'username' => 'your_LimeSurvey_database_username',"
  . "\n\t\t" . '<br />' . "'password' => 'your_LimeSurvey_database_user_password',"
  . "\n\t\t" . '<br />' . "'host' => 'your_LimeSurvey_host'"
  . "\n\t\t" . '<br />' . "'prefix' => '<strong>lime_</strong>' [" . t('or another value') . "]"
  . "\n\t\t" . '<br />' . "'collation' => 'utf8_general_ci' [" . t('or another value') . "]"
  . "\n\t\t" . '<br />' . ");"
  . "\n\t\t" . '</blockquote>'
  . "\n\t\t" . $replacing_string;
  $server_list[] = t('Concerning the <em>LimeSurvey database prefix</em> value,') . ' ' . $find_it_string;
  $server_list[] = t('Note that if you host your LimeSurvey site and your Drupal site on the same database, you have to choose distinct prefix.');
  $server_help = theme('item_list', array('items' => $server_list));

  $test_result['server'] = array(
    'title' => t('The LimeSurvey database connection informations'),
    'result' => $server_ok,
    'message' => $server_output,
    'help' => $server_help,
  );

  // Test if one LimeSurvey database table can be reached.
  if (!$server_ok) {
    $table_ok = FALSE;
    $table_output = $server_fails_comment;
    $table_help = $server_fails_help;
  }
  else {
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
    $previous_db = limesurvey_sync_db_open();
    $table_ok = db_table_exists(limesurvey_sync_table());
    limesurvey_sync_db_close($previous_db);
    $table_output = ($table_ok) ? t("The 'surveys' database table of the LimeSurvey site has been successfully found.") : t("The 'surveys' database table of the LimeSurvey site has not been found.");
    $table_help = "\n" . t('If it fails, yo may need to check :');
    $table_list = array();
    $table_list[] = t('either if you have correctly installed the !link_ls,', array('!link_ls' => l(t('LimeSurvey software'), 'http://www.limesurvey.org', $windows_tag)));
    $table_list[] = t('either your LimeSurvey database connection informations,');
    $table_list[] = t('or the <em>LimeSurvey database prefix</em> value :') . ' ' . $find_it_string;
    $table_help .= theme('item_list', array('items' => $table_list));
  }

  $test_result['table'] = array(
    'title' => t('The LimeSurvey site database tables'),
    'result' => $table_ok,
    'message' => $table_output,
    'help' => $table_help,
  );

  // Test the LimeSurvey url.
  // Don't use urlencode() because 'http://*' returns 'http%3A%2F%2F*' !
  $url_base = str_replace(' ', '%20', limesurvey_sync_rel2abs($settings['ls_path']));
  // Need php5 for that testing headers values. Drupal 7 runs only under php5.

  if (!ini_get('allow_url_fopen')) {
    // To allow the treatment of URLs (like http:// or ftp://) as files.
    // ini_set('allow_url_fopen', 1);
    // Ini_set() function fails. The raison maybe because If Drupal set
    // something using php_admin_value in httpd.conf it is then not possible
    // to be set the value at runtime.
    $tested_url_output = t("<em>allow_url_fopen</em> should be set to '<em>On</em>' into the <em>php.ini</em> file for this test.")
    . '<br /><strong>' . t('Test abort.') . '</strong><br />'
    . t('You need to manually test the !url URL, making sure you can reach your LimeSurvey site.', array('!url' => filter_xss(l($url_base, $url_base, $windows_tag))));
    $url_ok = NULL;
  }
  else {
    $ls_html_admin = $version = $httpcode = $msg = '';
    $admin_login_page = (limesurvey_sync_lsversion() != '1.x') ? $admin_login_page_2x : $admin_login_page_1x;
    $url_admin = $url_base . $admin_login_page;
    $tested_url = $url_admin;
    $tested_url_redirected = limesurvey_sync_find_redirection_url($url_admin);
    // Retrieve HTTP status code :
    $http_response_header = @get_headers($tested_url_redirected);
    if (!empty($http_response_header)) {
      list($version, $httpcode, $msg) = explode(' ', $http_response_header[0], 3);
      $ls_html_admin = @file_get_contents($tested_url_redirected);
    }
    if (empty($httpcode)) {
      // Some configurations fail here. As I don't know why : returns NULL
      // instead of FALSE, because FALSE breaks the form validation.
      $reached = NULL;
      $error_status = t('Undocumented error');
    }
    else {
      $reached = ($httpcode >= 200 && $httpcode < 400);
      // Check the HTTP Status code :
      if (!$reached) {
        switch ($httpcode) {
          case 401:
          case 403:
            $error_status = t("Error 401: unauthorized request");
            break;
          case 400:
            $error_status = t("Error 400: invalid request");
            break;
          case 404:
            $error_status = t("Error 404: page not found");
            break;
          case ($status_code >= 500):
            $error_status = t("Error %num_error: server replied with an error", array('%num_error' => $httpcode));
            break;
          default:
            $error_status = t("Undocumented error : @mess #@error", array('@mess' => $msg, '@error' => $httpcode));
            break;
        }
      }
      else {
        // The page exists.
        // The word : "LimeSurvey" is on the footer aera in the login admin
        // form page.
        $find_ls_text = (($ls_html_admin !== FALSE && stripos($ls_html_admin, 'LimeSurvey') !== FALSE));
        // Check if the connected Limesurvey database match the same LimeSurvey
        // install folder via its url.
        $find_ls_text_sid = TRUE;
        if ($table_ok) {
          $fake_sid = limesurvey_sync_fake_survey();
          $ls_url_fake_survey = $url_base . $fake_survey_prefix;
          $query_url = array('absolute' => TRUE);
          if ((limesurvey_sync_lsversion() != '1.x') ) {
            $ls_url_fake_survey .= '/survey/index/sid/' . $fake_sid;
          }
          else {
            $query_url['query']['sid'] = $fake_sid;
          }
          $ls_url_fake_survey_sid = url($ls_url_fake_survey, $query_url);
          $ls_html_fakesurvey = @file_get_contents($ls_url_fake_survey_sid);
          // Look for the survey name '#245' for example.
          $find_ls_text_sid = (($ls_html_fakesurvey !== FALSE && strpos($ls_html_fakesurvey, '#' . $fake_sid) !== FALSE));
          // Delete the fake_survey.
          limesurvey_sync_fake_survey($fake_sid, 'delete');
        }
      }
    }

    if ($reached && $find_ls_text) {
      $url_ok = ($find_ls_text_sid); // TRUE or FALSE.
      $tested_url_output = ($find_ls_text_sid) ? t('The LimeSurvey site page has been successfully reached') : t('A LimeSurvey site page has been successfully reached, <strong>but it does not match the same LimeSurvey database</strong>.');
    }
    elseif ($reached) {
      $tested_url_output = t("This page does not contain the word 'LimeSurvey'");
      $url_ok = FALSE;
    }
    else {
      $tested_url_output = t('The LimeSurvey site page has not been reached (%error)', array('%error' => $error_status));
      $url_ok = ($reached === NULL) ? NULL : FALSE;
    }

    $displayed_url = array('!url' => filter_xss(l($tested_url, $tested_url, $windows_tag)));
    if ($tested_url_redirected == $tested_url) {
      $tested_url_output .= '<br />' . t('Tested url is :<br />!url.', $displayed_url);
    }
    else {
      $tested_url_output .= '<br />' . t('Tested url is :<br />!url, redirected to !redirected_url.', array_merge($displayed_url, array('!redirected_url' => filter_xss(l($tested_url_redirected, $tested_url_redirected, $windows_tag)))));
    }
  }

  $test_result['url'] = array(
    'title' => t('The LimeSurvey site url'),
    'result' => $url_ok,
    'message' => $tested_url_output,
    'help' => t('If it fails, you may need to check the <em>LimeSurvey site path</em> setting and save it on the current page.'),
  );

  // Check for the date timezone on both server.
  $ls_gset_text = t('LimeSurvey global setting page');
  $link_ls_time = ($url_ok) ? limesurvey_sync_link_to_ls($ls_gset_text, NULL, 'globalsettings', 'general') : $ls_gset_text;
  if (!$server_ok && !$table_ok) {
    $timezone_ok = FALSE;
    $timezone_output = t('The LimeSurvey site hosting server and the LimeSurvey site database tables tests must succeed in order to run this test.');
    $timezone_help = t('You need to fix both tests first.');
  }
  elseif (!$server_ok) {
    $timezone_ok = FALSE;
    $timezone_output = $server_fails_comment;
    $timezone_help = $server_fails_help;
  }
  elseif (!$table_ok) {
    $timezone_ok = FALSE;
    $timezone_output = t('The LimeSurvey site database tables test must succeed in order to run this test.');
    $timezone_help = t('You need to fix the test of the LimeSurvey site database tables first.');
  }
  else {
    // Here we check for of a LimeSurvey issue
    //(http://bugs.limesurvey.org/view.php?id=6038).
    // http://www.php.net/manual/en/function.date-default-timezone-get.php
    // LimeSurvey uses the code (on limesurvey/common.php line 89 :
    // @date_default_timezone_set(@date_default_timezone_get());
    // to set the timezone.

    // On Drupal 6, the timezone is returned as seconds, but in Drupal 7, it is
    // returned as name (ex: Europe/Paris).
    // Retrieve 4 datas : The Drupal and LimeSurvey servers and sites
    // timezones.
    // Retrieve the original date_default_timezone_get() value before Drupal
    // set it.
    // http://www.php.net/manual/en/function.date-default-timezone-get.php
    $uncheckable_tz = FALSE;
    $prior_php54 = (version_compare(PHP_VERSION, '5.4.0', '<'));
    if ($prior_php54 && !empty($_SERVER['TZ'])) {
      $tz_dr_server = $_SERVER['TZ'];
    }
    elseif (ini_get('date.timezone')) {
      $tz_dr_server = ini_get('date.timezone');
    }
    elseif ($prior_php54) {
      // Querying the host operating system, if supported and allowed by the OS.
      // The value can not be retrieve. Set it arbitrarily to UTC.
      $tz_dr_server = 'UTC';
      $uncheckable_tz = TRUE;
    }
    else {
      $tz_dr_server = 'UTC';
    }
    $tz_dr_server_h = limesurvey_sync_get_timezone_offset($tz_dr_server) / 3600;
    $tz_dr_site = variable_get('date_default_timezone', @date_default_timezone_get());
    // Note : if the 'date_default_timezone' variable is not set, the returned
    // value will be the user timezone. If this user does not use the default
    // timezone, this will break the test.
    $tz_dr_site_h = limesurvey_sync_get_timezone_offset($tz_dr_site) / 3600;
    // Retrieve the limeSurvey timeadjust.
    $ls_gs = limesurvey_sync_ls_global_settings();
    // Convert '-12 hours' into -12.
    $tz_ls_adjust_h = (int) $ls_gs['timeadjust'];
    if (!in_array('host', $distinct_storage)) {
      // LimeSurvey and Drupal are using the same server.
      $tz_ls_server_h = $tz_dr_server_h;
    }
    else {
      // LimeSurvey and Drupal do not use the same server. Assume that both
      // servers use the same configuration.
      $tz_ls_server_h = $tz_dr_server_h;
      $uncheckable_tz = TRUE;
    }
    // Calculate the LimeSurvey using timezone, fixed by the LimeSurvey global
    // setting adjust time value.
    $tz_ls_site_h = $tz_ls_server_h + $tz_ls_adjust_h;
    // Compare both timezones.
    $delta_tz = $tz_ls_site_h - $tz_dr_site_h;
    // Delete minuts if any.
    $same_tz = (round($delta_tz) == 0);
    // Calculate the correct LimeSurvey adjust time.
    $calc_adjut_time = $tz_dr_site_h - $tz_ls_server_h;
    if (!$uncheckable_tz) {
      if ($same_tz) {
        $timezone_ok = TRUE;
        $timezone_output = t('Your Drupal site and your LimeSurvey site are using the same timezone setting.');
      }
      else {
        // Only warning, this test is weak, depending on server configuration.
        $timezone_ok = NULL;
        $timezone_output = '<strong>' . t('Your Drupal site and your LimeSurvey site are not using the same timezone setting.') . '</strong>';
      }
    }
    else {
      $timezone_ok = NULL;
      if ($same_tz) {
        $timezone_output = t('Your Drupal site and your LimeSurvey site seem to use the same timezone setting.');
      }
      else {
        $timezone_output = '<strong>' . t('Your Drupal site and your LimeSurvey site seem to not use the same timezone setting.') . '</strong>';
      }
    }

    $timezone_help = t('If it does not succeed, you may check :');
    $tz_list = array();
    $tz_list[] = t('your !link_drupal_tz', array('!link_drupal_tz' => l(t('Drupal default timezone setting'), 'admin/config/regional/settings', $windows_tag)));
    $tz_list[] = t('and the <em>Corrected time</em> value on your !link_ls_tz; edit the <em>Time difference</em> parameter in order to match the LimeSurvey <em>Corrected time</em> value to the Drupal <em>default time zone</em> value.', array('!link_ls_tz' => $link_ls_time))
    . '<br />' . t('If your Drupal default timezone setting is correctly set, the <em>Time difference</em> should be equal to %td. In any case, the <em>Corrected time</em> from the LimeSurvey admin interface is the value to be trusted.', array('%td' => ($calc_adjut_time == 0) ? t('0 hour') : format_plural($calc_adjut_time, '@count hour', '@count hours')));
    $timezone_help .= theme('item_list', array('items' => $tz_list));
    $timezone_help .= t('Note : on Daylight Saving Time changes, you may need to edit both values.');
  }

  $test_result['timezone'] = array(
    'title' => t('LimeSurvey site Timezone setting'),
    'result' => $timezone_ok,
    'message' => $timezone_output,
    'help' => $timezone_help,
  );

  // Check for the date on both server
  if (!$server_ok) {
    $time_ok = FALSE;
    $tested_delta_time_output = $server_fails_comment;
    $tested_delta_time_help = $server_fails_help;
  }
  else {
    $delta_time = limesurvey_sync_calculate_server_time_diff();
    $delta_time_saved = $settings['ls_delta_time'];
    $time_ok = ($delta_time_saved == $delta_time) ? TRUE : NULL;
    $tested_delta_time_output = ($time_ok) ? t('Time on the Drupal server and on the LimeSurvey server is synchronized.') :  t('Time on the Drupal server and on the LimeSurvey server is not synchronized.');
    $tested_delta_time_help = t('If it does not succeed, you may set the <em>adjust time</em> setting to %delta seconds and save it on the current page.', array('%delta' => ($delta_time !== FALSE) ? $delta_time : t('undefined')));
  }

  $test_result['time'] = array(
    'title' => t('Synchronized servers'),
    'result' => $time_ok,
    'message' => $tested_delta_time_output,
    'help' => $tested_delta_time_help,
  );

  return $test_result;
}

/**
 * Format an custom url in the same way as the url() function will do.
 *
 * This function is used to compare custom url with url generate by the url()
 * function. It takes care of the clean url. For example, convert the url :
 * 'http://www.example.com/?q=page' to 'http://www.example.com/page'.
 *
 * @param $url
 *   A string. The url to format.
 *
 * @return
 *   A string. The formated absolute url.
 */
function limesurvey_sync_drupal_formated_url($url) {
  $queries = array();
  if (strpos($url, '?') !== FALSE) {
    $parse_url = @parse_url($url);
    // Handle unique or multiple arguements.
    $parse_query = (strpos($parse_url['query'], '&') !== FALSE) ? explode('&', $parse_url['query']) : array($parse_url['query']);
    foreach ($parse_query as $arg) {
      $delimiter = strpos($arg, '=');
      if ($delimiter !== FALSE) {
        $queries[drupal_substr($arg, 0, $delimiter)] = drupal_substr($arg, $delimiter + 1);
      }
    }

    // Handle the q arguement.
    if (!empty($queries) && array_key_exists('q', $queries)) {
      $q_query = $queries['q'];
      unset($queries['q']);
      // Handle clean urls.
      if (variable_get('clean_url', '0') && array_key_exists('q', $queries)) {
        $parse_url['path'] .= $q_query;
      }
      else {
        // Need to set the q arg as the first arguement.
        $queries = array_merge(array('q' => $q_query), $queries);
      }
    }
    $scheme = (!empty($parse_url['scheme'])) ? $parse_url['scheme'] . '://' : '';
    $fragment = (!empty($parse_url['fragment'])) ? array('fragment' => $parse_url['fragment']) : array();
    $url = url($scheme . $parse_url['host'] . $parse_url['path'], array_merge(array('query' => $queries), $fragment));
  }
  // Return the absolute url.
  return $url;
}

/**
 * Convert relative url to absolute url.
 *
 * Code from http://stackoverflow.com/a/1243431.
 *
 * @param $rel
 *   A string. The relative url.
 * @param $base
 *   A string. The base url. Default is empty corresponding to the drupal site
 *   base url.
 *
 * @return
 *   A string, the converted absolute url.
 */
function limesurvey_sync_rel2abs($rel, $base = NULL) {
  // Set the base url.
  if (empty($base)) {
    global $base_url;
    $base = $base_url . '/';
  }

  if (empty($rel)) {
    return $base;
  }

  // Return if already absolute URL.
  // Removethe PHP_URL_SCHEME component and add @ to make this function
  // compatible with php < 5.1.2.
  $rel_scheme = @parse_url($rel);
  if (!empty($rel_scheme['scheme'])) {
    return $rel;
  }

  // Queries and anchors.
  if ($rel[0] == '#' || $rel[0] == '?') {
    return $base . $rel;
  }

  // Parse base URL and convert to local variables:  $scheme, $host, $path.
  extract(@parse_url($base));

  // Remove non-directory element from path.
  $path = preg_replace('#/[^/]*$#', '', $path);

  // Destroy path if relative url points to root.
  if ($rel[0] == '/') {
    $path = '';
  }

  $port_url = (!empty($port)) ? ':' . $port : '';
  // Dirty absolute URL.
  $abs = "$host$port_url$path/$rel";

  // Replace '//' or '/./' or '/foo/../' with '/'.
  $re = array('#(/\.?/)#', '#/(?!\.\.)[^/]+/\.\./#');
  for ($n = 1; $n > 0; $abs = preg_replace($re, '/', $abs, -1, $n)) {
  }
  // Absolute URL is ready!
  return (!empty($scheme)) ? $scheme . '://' . $abs : $abs;
}

/**
 * Return an image of a boolean value.
 *
 * @param $bool
 *   A boolean or the NULL value for 'warning'.
 * @param $center
 *   A boolean. If TRUE, align on center Default is TRUE.
 *
 * @return
 *   A string, the html image for TRUE, FALSE or WARNING.
 */
function limesurvey_sync_display_boolean_img($bool, $center = TRUE) {
  $align = ($center) ? ' align="center"' : '';
  $variable = array(
    'width' => 18,
    'height' => 18,
  );

  if ($bool === NULL) {
    $variable['path'] = 'misc/watchdog-warning.png';
    $variable['alt'] = $variable['title'] = t('Warning');
  }
  elseif ($bool) {
    $variable['path'] = 'misc/watchdog-ok.png';
    $variable['alt'] = $variable['title'] = t('Successful');
  }
  else {
    $variable['path'] = 'misc/watchdog-error.png';
    $variable['alt'] = $variable['title'] = t('Failure');
  }

  return '<div' . $align . '>' . theme('image', $variable) . '</div>';
}

/**
 * Returns the offset from the remote timezone to the origin timezone, in seconds.
 *
 * Note :this function works only for PHP >= '5.2.0', but Drupal 7 needs at
 * least PHP '5.2.4'.
 *
 * @param $remote_tz
 *   A string, the remote timezone (ex : 'America/Los_Angeles').
 * @param $origin_tz
 *   A string, if NULL the servers current timezone is used as the origin.
 *   Default is 'UTC'.
 *
 * @return
 *   An integer, the offset in seconds.
 */
function limesurvey_sync_get_timezone_offset($remote_tz, $origin_tz = 'UTC') {
  if ($origin_tz === NULL) {
    if (!is_string($origin_tz = date_default_timezone_get())) {
      return FALSE; // A UTC timestamp was returned -- bail out!
    }
  }
  $origin_dtz = new DateTimeZone($origin_tz);
  $remote_dtz = new DateTimeZone($remote_tz);
  $origin_dt = new DateTime("now", $origin_dtz);
  $remote_dt = new DateTime("now", $remote_dtz);
  $offset = $remote_dtz->getOffset($remote_dt) - $origin_dtz->getOffset($origin_dt);
  return $offset;
}

/**
 * Disable the overlay.
 *
 * The overlay core module doesn't like external urls
 * (issue : http://drupal.org/node/1152314) neither link opening into a new
 * window. The LimeSurvey sync module checks the http_referrer value in order
 * to allow to access to the LimeSurvey form page. The overlay is not
 * compatible with such feature. Disable it for the LimeSurvey synchronization
 * setting page, the answer node edit page and the answers node type edit page.
 */
function limesurvey_sync_disable_overlay() {
  // Code adapted from the overlay_init() core function.
  global $user;
  $use_overlay = !isset($user->data['overlay']) || $user->data['overlay'];
  if (module_exists('overlay') && drupal_substr(request_uri(), -15) == '?render=overlay' && user_access('access overlay') && $use_overlay) {
    $current_path = current_path();
    // Disable the overlay.
    overlay_close_dialog($current_path, array('query' => drupal_get_query_parameters(NULL, array('q', 'render'))));
  }
}

/**
 * Returns the ultimate url if redirections exist for a provided url.
 *
 * @param $url
 *   A string, the provided url.
 *
 * @return
 *   A string, the ultimate url or the same url if curl is not enabled or if
 *   there is no redirection.
 */
function limesurvey_sync_find_redirection_url($url) {
  // Code from http://stackoverflow.com/a/11699301.
  if (function_exists('curl_version')) {
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HEADER, TRUE); // We'll parse redirect url from header.
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, FALSE); // We want to just get redirect url but not to follow it.
    $response = curl_exec($ch);
    preg_match_all('/^Location:(.*)$/mi', $response, $matches);
    curl_close($ch);
    if (!empty($matches[1])) {
      // Handle multiple redirections. As an example : a LimeSurvey v1 database
      // targeting a LimeSuvrey v2 site :
      // Provided patch by the user is 'limesurvey',
      // $url is 'http://example.com/limesurvey/admin/admin.php',
      // redirected to : 'http://example.com/limesurvey/index.php/admin' with
      // [location = '../index.php/admin'], redirected again to :
      // 'http://example.com/limesurvey/index.php/admin/authentication/login'
      // with [location =
      // 'http://example.com/limesurvey/index.php/admin/authentication/login'].
      return limesurvey_sync_find_redirection_url(str_replace(' ', '%20', limesurvey_sync_rel2abs(trim($matches[1][0]), $url)));
    }
  }
  return $url;
}
