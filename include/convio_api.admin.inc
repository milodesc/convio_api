<?php

function convio_api_admin_site_settings() {
  $form = array();
  
  $form['convio_api_site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of Organization'),
    '#description' => t('Please enter the site name'),
    '#default_value' => variable_get('convio_api_site_id', ''),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['convio_api_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API Base URL'),
    '#description' => t('The base URL for the API calls. Different Convio customers have different base URLs (e.g., https://secure2.convio.net/ or https://secure3.convio.net/).'),
    '#default_value' => variable_get('convio_api_site_url', 'https://secure2.convio.net/'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );
  
  $form['convio_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('API key to use the Convio API'),
    '#default_value' => variable_get('convio_api_key', ''),
    '#size' => 64,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  
  $form['convio_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('API Username'),
    '#description' => t('Special administrative account used just for API access'),
    '#default_value' => variable_get('convio_api_username', ''),
    '#size' => 64,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  
  $form['convio_api_password'] = array(
    '#type' => 'password',
    '#title' => t('API Password'),
    '#required' => TRUE,
  );
  
  $form = system_settings_form($form);
  return $form;
}

/**
 * Implements hook_form_validate
 */
function convio_api_admin_site_settings_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!preg_match('/^https:\/\//', $values['convio_api_site_url']))
  {
    // We do not want to send our example request over http!
    form_set_error('convio_api_site_url', 'Site URL *must* begin with https://');
  }
}

function convio_api_admin_site_test() {
    $form = array();

    $form['convio_api_email'] = array(
        '#type' => 'textfield',
        '#title' => t('E-Mail'),
        '#required' => TRUE,
        '#description' => t('An e-mail address to test for validity (e.g. test@example.com).'),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Run test'),
    );

    return $form;
}

/**
 * Implements hook_form_validate
 */
function convio_api_admin_site_test_validate($form, &$form_state) {
    $site_id = variable_get("convio_api_site_id");
    $site_url = variable_get("convio_api_site_url");
    $api_key = variable_get("convio_api_key");
    $api_username = variable_get("convio_api_username");
    $api_password = variable_get("convio_api_password");

    if(empty($site_id) || empty($site_url) || empty($api_key) || empty($api_username) || empty($api_password)) {
        form_set_error('submit', "Please fully configure your Convio API connection on the Convio API Configuration tab before testing your connection.");
    }
}

/**
 * Implements hook_form_submit
 */
function convio_api_admin_site_test_submit($form, &$form_state) {
  try {
    $response = convio_api_isEmailValid_request($form_state['values']['convio_api_email']);
  }
  catch (Exception $e) {
    form_set_error('', $e->getMessage());
    return;
  }

  $validity = ($response['isEmailValidResponse']['valid'] === 'true') ? "a valid" : "an invalid";
  drupal_set_message('Connection to Convio API was successful. ' . $form_state['values']['convio_api_email'] . " is " . $validity . " email address.");
}
