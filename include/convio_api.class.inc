<?php

class ConvioAPI {
  private $api;
  private $api_data = array(
    'address_book' => array('path' => 'AddressBookAPI', 'supported_methods' => array('client')),
    'advocacy' => array('path' => 'AdvocacyAPI', 'supported_methods' => array('client', 'server')),
    'constituent' => array('path' => 'ConsAPI', 'supported_methods' => array('client', 'server')),
    'content' => array('path' => 'ContentAPI', 'supported_methods' => array('client', 'server')),
    'datasync' => array('path' => 'DataSyncAPI', 'supported_methods' => array('client', 'server')),
    'donation' => array('path' => 'DonationAPI', 'supported_methods' => array('client', 'server')),
    'event' => array('path' => 'OrgEventAPI', 'supported_methods' => array('client', 'server')),
    'group' => array('path' => 'GroupAPI', 'supported_methods' => array('client', 'server')),
    'single_sign_on' => array('path' => 'ConsAPI', 'supported_methods' => array('client', 'server')),
    'survey' => array('path' => 'SurveyAPI', 'supported_methods' => array('client')),
    'teamraiser' => array('path' => 'TeamraiserAPI', 'supported_methods' => array('client', 'server')),
  );
  private $method;
  private $config;
  private $params;
  private $request_url;
  
  public function __construct($api, $method, $required_params, $optional_params) {
    if (!empty($this->api_data[$api])) {
      $this->api = $api;
      if (in_array($method, $this->api_data[$api]['supported_methods'])) {
        $this->method = $method;
        $this->get_configuration();
        $this->get_params($required_params, $optional_params);
        $this->get_request_url();
      }
      else {
        throw new Exception("API $api does not support method $method");
      }
    }
    else {
      throw new Exception("API $api is not supported.");
    }
  }

  /**
   * Build the configuration array for a convio api request.
   *
   * @return array
   *   An array containing the configuration for a Convio API request.
   *   Includes the keys:
   *     - convio_api_username
   *     - convio_api_password
   *     - convio_api_key
   *     - convio_api_site_id
   *     - convio_api_site_url
   *
   */
  private function get_configuration() {
    $this->config = array(
      'convio_api_username' => variable_get('convio_api_username', ''),
      'convio_api_password' => variable_get('convio_api_password', ''),
      'convio_api_key' => variable_get('convio_api_key', ''),
      'convio_api_site_id' => variable_get('convio_api_site_id', ''),
      'convio_api_site_url' => variable_get('convio_api_site_url', 'https://secure2.convio.net/'),
    );
  }

  private function get_params($required_params, $optional_params) {
    $params = $required_params;
    $params += array(
      'api_key' => $this->config['convio_api_key'],
      'v' => '1.0',
      'response_format' => 'json'
    );

    if ($this->method !== 'client') {
      $params += array(
        'login_name' => $this->config['convio_api_username'],
        'login_password' => $this->config['convio_api_password'],
      );
    }

    $params = array_merge($params, $optional_params);

    $this->params = $params;
  }

  private function get_request_url() {
    $method_path = ($this->method === 'client') ? 'CR' : 'SR';
    $this->request_url = $this->config['convio_api_site_url'] . $this->config['convio_api_site_id'] . '/site/' . $method_path . $this->api_data[$this->api]['path'];
  }
  
  public function execute() {
    $headers = array(
      'Content-Type' => 'application/x-www-form-urlencoded',
    );
    
    return drupal_http_request($this->request_url, array('headers' => $headers, 'method' => 'POST', 'data' => http_build_query($this->params, '', '&')));
  }
}
