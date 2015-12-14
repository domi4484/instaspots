<?php

namespace Instaspots\SpotsBundle\Controller;

class Response
{
  private $command = '';

  private $clientVersion = '';

  private $data = array();

  private $error   = '';
  private $warning = '';

  //-----------------------------------------------------------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------------------------------------------------

  /**
   * Constructor
   */
  public function __construct()
  {

  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function setCommand($command)
  {
    $this->command = $command;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function getCommand()
  {
    return $this->command;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function setClientVersion($clientVersion)
  {
    $this->clientVersion = $clientVersion;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function getClientVersion()
  {
    return $this->clientVersion;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function setError($error)
  {
    $this->error = $error;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function setWarning($warning)
  {
    $this->warning = $warning;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function addData($key,
                          $value)
  {
    $this->data[$key] = $value;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function getData($key)
  {
    return $this->data[$key];
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function toJson()
  {
    $this->data['command']  = $this->command;
    $this->data['error']    = $this->error;

    if (strlen($this->warning) != 0)
    {
      $this->data['warning'] = $this->warning;
    }

    return $this->data;
  }
}
