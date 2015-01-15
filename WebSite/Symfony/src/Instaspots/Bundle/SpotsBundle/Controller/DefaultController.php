<?php

namespace Instaspots\Bundle\SpotsBundle\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;

class DefaultController extends Controller
{
    public function indexAction($name)
    {
        return $this->render('InstaspotsSpotsBundle:Default:index.html.twig', array('name' => $name));
    }
}
