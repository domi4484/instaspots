<?php

namespace Instaspots\SpotsBundle\Controller;

use Instaspots\UserBundle\Entity\User;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\HttpFoundation\Response;

class AdvertController extends Controller
{
  public function indexAction()
  {
    $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsSpotsBundle:Picture');

    $pictures = $repository->getNews();

    return $this->render('InstaspotsSpotsBundle:Advert:index.html.twig', 
                         array('listPictures' => $pictures));
  }
  
  public function downloadAction()
  {
    $listPlatforms = array(
      array('title' => 'Android', 'link' => 'link')
    );
    
    return $this->render('InstaspotsSpotsBundle:Advert:download.html.twig', 
                         array('listPlatforms' => $listPlatforms));
  }
  
  public function viewAction($id)
  {
    $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsSpotsBundle:Spot');
    
    $spot = $repository->find($id);
    
    if (null === $spot) {
      throw new NotFoundHttpException("L'annonce d'id ".$id." n'existe pas.");
    }

    return new Response("Affichage de l'annonce d'id : ".$spot->getName());
  }
  
  public function menuAction($limit)
  {
    // On fixe en dur une liste ici, bien entendu par la suite
    // on la récupérera depuis la BDD !
    $listAdverts = array(
      array('id' => 2, 'title' => 'Recherche développeur Symfony2'),
      array('id' => 5, 'title' => 'Mission de webmaster'),
      array('id' => 9, 'title' => 'Offre de stage webdesigner')
    );

    return $this->render('InstaspotsSpotsBundle:Advert:menu.html.twig', array(
      // Tout l'intérêt est ici : le contrôleur passe
      // les variables nécessaires au template !
      'listAdverts' => $listAdverts
    ));
  }
}

