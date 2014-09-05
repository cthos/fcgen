<?php
/**
 * @file Holds common classes for the FCGen migration.
 */

use Faker;

class FCGenBaseMigration extends Migration {
  protected $driver;

  public function __construct($arguments) {
    // Generation defaults
    $defaults = array(
      'number_to_generate' => 1000,
      'driver' => 'faker',
    );

    $arguments = array_merge($defaults, $arguments);

    $this->driver = FCGenDriver::factory($arguments['driver']);
    $this->source = $this->driver->getSource($arguments['number_to_generate']);
  }
}