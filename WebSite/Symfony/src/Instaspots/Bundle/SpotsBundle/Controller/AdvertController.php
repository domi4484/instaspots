<?php

namespace Instaspots\Bundle\SpotsBundle\Controller;

use Symfony\Component\HttpFoundation\Response;

class AdvertController
{
  public function indexAction()
  {
    return new Response("Ciao mondello!");
  }
}

