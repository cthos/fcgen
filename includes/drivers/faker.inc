<?php

class FCGenDriverFaker extends FCGenDriver {
  protected $faker;
  protected static $instance;

  public static function getInstance() {
    if (!isset(static::$instance)) {
      static::$instance = new static;
    }

    return static::$instance;
  }

  /**
   * Gets the key schema:
   *
   * @todo This might need to actually be a serial rng or unique.
   *
   * @return array
   */
  public function getKeySchema() {
    return MigrateSourceFaker::getKeySchema();
  }

  public function init($arguments = null) {
    // Sensible Defaults:
    $faker_defaults = array(
      'language' => 'en_US',
      'seed'     => NULL,
    );

    if (empty($arguments)) {
      $arguments = array();
    }

    $arguments = array_merge($faker_defaults, $arguments);

    // TODO: This might need to be per content type, but also exportable.
    if (!$this->seed = variable_get('fcgen_faker_seed', $arguments['seed'])) {
      $this->seed = $this->generateSeed();
    }

    $this->faker = Faker\Factory::create();
    $this->faker->seed($this->seed);
  }

  public function getSource($number_to_generate) {
    return new MigrateSourceFaker($number_to_generate, array('faker' => $this->faker));
  }

  /**
   * Shim method around calling faker, to abstract out the driver system.
   *
   * That way you can call $driver->generate and the outer class doesn't need
   * to know about faker necessarily (though granted, I'm using faker's naming
   * conventions so other drivers would need to account for that).
   *
   * Accepts a $type and $arguments. Uses func_get_args().
   *
   * @return mixed
   */
  public function generate() {
    $arguments = func_get_args();
    $type = array_shift($arguments);

    if (empty($arguments)) {
      $arguments = array();
    }

    // Just pass the call right along to Faker:
    if (!empty($arguments)) {
      // TODO: This is kinda messy
      if ($type == 'unique') {
        return $this->faker->unique()->{$arguments[0]};
      }

      return call_user_func_array(array($this->faker, $type), $arguments);
    }

    return $this->faker->{$type};
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