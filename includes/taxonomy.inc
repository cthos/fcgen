<?php
/**
 * @file Holds the taxonomy base class for FCGen
 */

/**
 * Class FCGenTaxonomyMigration
 */
class FCGenTaxonomyMigration extends FCGenBaseMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    if (empty($this->arguments['bundle'])) {
      throw new \Exception("Must specify bundle for the target Taxonomy Vocabulary");
    }

    $this->destination = new MigrateDestinationTerm($this->arguments['bundle']);
    $this->map = new MigrateSQLMap(
      $this->machineName,
      $this->driver->getKeySchema(),
      MigrateDestinationTerm::getKeySchema()
    );

    $this->addFieldMapping('name', 'word');
    $this->addFieldMapping('description', 'text');

    if (!empty($this->arguments['hierarchical'])) {
      $this->addFieldMapping('parent')->callbacks(array($this, 'getRandomParent'));
    }
  }
}