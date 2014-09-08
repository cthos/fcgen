<?php


class FCGenExampleFoodMigration extends FCGenNodeMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $driver = $this->driver;

    $this->addFieldMapping('field_food_type', 'llama')->callbacks(function ($value) use ($driver) {
      return $driver->generate('numberBetween', 0, 10);
    });
  }
}