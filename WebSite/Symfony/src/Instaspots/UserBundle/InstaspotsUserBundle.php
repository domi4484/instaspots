<?php

namespace Instaspots\UserBundle;

use Symfony\Component\HttpKernel\Bundle\Bundle;

class InstaspotsUserBundle extends Bundle
{
    public function getParent()
    {
        return 'FOSUserBundle';
    }
}
