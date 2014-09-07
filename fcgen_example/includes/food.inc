<?php


class FCGenFoodMigration extends FCGenNodeMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);


    $this->addFieldMapping('field_food_type')->callbacks(function () {
      $this->driver->generate('numberBetween', 0, 10);
    });
  }
}