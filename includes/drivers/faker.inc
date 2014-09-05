<?php

use Faker;

class FCGenDriverFaker extends FCGenDriver {
  protected $faker;
  protected static $instance;

  public static function getInstance() {
    if (!isset(static::$instance)) {
      static::$instance = new static;
    }

    return static::$instance;
  }

  public function getKeySchema() {
    return array(
      'randomNumber' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    );
  }

  public function init($arguments = null) {
    // Sensible Defaults:
    $faker_defaults = array(
      'language' => 'en',
      'seed'     => NULL,
    );

    if (empty($arguments)) {
      $arguments = array();
    }

    $arguments = array_merge($faker_defaults, $arguments);

    if (!$this->seed = variable_get('fcgen_faker_seed', $arguments['seed'])) {
      $this->seed = $this->generateSeed();
    }

    $this->faker = Faker\Factory::create();
    $this->faker->seed($this->seed);
  }

  public function getSource($number_to_generate) {
    return new MigrateSourceFCGen($number_to_generate, array('faker' => $this->faker));
  }

  public function generate($type, array $arguments = null) {
    if (empty($arguments)) {
      $arguments = array();
    }

    // Just pass the call right along to Faker:
    $this->faker->{$type};
  }

  /**
   * Generates a faker seed and stores it as a variable.
   *
   * This way you get consistent content across the board.
   *
   * @return int
   */
  protected function generateSeed() {
    // This can be more complicated if necessary.
    $seed =  mt_rand();

    variable_set('fcgen_faker_seed', $seed);

    return $seed;
  }
}