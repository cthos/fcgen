<?php

abstract class FCGenDriver {
  abstract public function init($arguments = null);

  abstract public function generate($type, array $arguments = null);

  abstract public function getKeySchema();

  public static function factory($driver = 'faker') {
    switch ($driver) {
      case 'faker':
      default:
        return FCGenDriverFaker::getInstance();
    }
  }
}