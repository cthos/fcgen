<?php


/**
 * Implements hook_migrate_api.
 *
 * @see migrate_examples_migrate_api for more information.
 */
function fcgen_example_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'fcgen_example' => array(
        'title' => t('FCGen Example'),
      ),
    ),
    'migrations' => array(
      'FCGenUser' => array(
        'class_name' => 'FCGenUserMigration',
        'group_name' => 'fcgen_example',
        'number_to_generate' => 2989,
      ),
      'FCGenPageMigration' => array(
        'user_migration' => 'FCGenUser',
        'bundle'         => 'page',
        'class_name'     => 'FCGenNodeMigration',
        'group_name'     => 'fcgen_example',
        'number_to_generate' => 20834,
      ),
      'FCGenFoodMigration' => array(
        'user_migration' => 'FCGenUser',
        'bundle'         => 'fcgen_food',
        'class_name'     => 'FCGenExampleFoodMigration',
        'group_name'     => 'fcgen_example',
      ),
      'FCGenForumTerm' => array(
        'user_migration' => 'FCGenUser',
        'bundle'         => 'forums',
        'class_name'     => 'FCGenExampleForumTermMigration',
        'group_name'     => 'fcgen_example',
        'hierarchical'   => TRUE,
      ),
      'FCGenForum' => array(
        'user_migration' => 'FCGenUser',
        'bundle'         => 'forum',
        'class_name'     => 'FCGenExampleForumMigration',
        'group_name'     => 'fcgen_example',
        'term_migration' => 'FCGenForumTerm',
        'number_to_generate' => 12900,
      )
    ),
  );
  return $api;
}