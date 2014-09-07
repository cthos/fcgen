<?php


class FCGenCommentMigration extends FCGenBaseMigration {

  public function __construct($arguments) {

    parent::__construct($arguments);

    if (empty($this->arguments['bundle'])) {
      throw new \Exception("Must specify bundle for node to which comments will be attached");
    }

    if (empty($this->arguments['source_migration'])) {
      throw new \Exception('Please pass source_migration as the machine name of the migration associated with the nodes to
       which these comments will be attached.');
    }

    $this->destination = new MigrateDestinationComment($this->arguments['bundle']);

    $this->map = new MigrateSQLMap(
      $this->machineName,
      $this->driver->getKeySchema(),
      MigrateDestinationComment::getKeySchema()
    );

    $this->addFieldMapping('nid')
         ->sourceMigration($this->arguments['source_migration'])
         ->callbacks(array($this, 'selectRandomFromSource'));

    $this->addFieldMapping('uid')
         ->sourceMigration($this->arguments['user_migration'])
         ->callbacks(array($this, 'selectRandomUser'));

    $this->addFieldMapping('pid')
         ->callbacks(array($this, 'getRandomParent'));

    $this->addFieldMapping('comment_body', 'text');
    $this->addFieldMapping('created', 'timestamp');
  }
}