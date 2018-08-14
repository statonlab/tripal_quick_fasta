<?php

namespace Tests;

use StatonLab\TripalTestSuite\DBTransaction;
use StatonLab\TripalTestSuite\TripalTestCase;


class FormTest extends TripalTestCase {

  // Uncomment to auto start and rollback db transactions per test method.
  use DBTransaction;

  public function test_tripal_quick_fasta_fetch_features_for_sequence_test() {
    $mrna_term = chado_get_cvterm(['id' => 'SO:0000234']);
    $prot_term = chado_get_cvterm(['id' => 'SO:0000104']);

    $mrna = factory('chado.feature')->create(['type_id' => $mrna_term->cvterm_id]);
    $protein = factory('chado.feature')->create(['type_id' => $prot_term->cvterm_id]);

    $features = tripal_quick_fasta_fetch_features_for_sequence();
    $this->assertNotEmpty($features);
  }

  public function test_tripal_quick_fasta_fetch_parent_mRNA_test() {

    $mrna_term = chado_get_cvterm(['id' => 'SO:0000234']);
    $prot_term = chado_get_cvterm(['id' => 'SO:0000104']);
    $type_term = chado_get_cvterm(['id' => 'SO:0000004']);//this is a random term....

    $mrna = factory('chado.feature')->create(['type_id' => $mrna_term->cvterm_id]);
    $protein = factory('chado.feature')->create(['type_id' => $prot_term->cvterm_id]);


    //add relationship

    db_insert('chado.feature_relationship')
      ->fields([
        'subject_id' => $protein->feature_id,
        'object_id' => $mrna->feature_id,
        'type_id' => $type_term->cvterm_id,
      ])
      ->execute();


    $parent_result = tripal_quick_fasta_fetch_parent_mRNA($protein->feature_id);
    $this->assertEquals($mrna->uniquename, $parent_result);

  }


  public function test_tripal_quick_fasta_get_parent_feature_types_test(){

    $term = factory('chado.cvterm')->create();
    factory('chado.feature')->create(['type_id' => $term->cvterm_id]);

    $types = tripal_quick_fasta_get_parent_feature_types();
    $this->assertNotEmpty($types);
    $this->assertArrayHasKey($term->cvterm_id, $types);
    $this->assertEquals($term->name, $types[$term->cvterm_id]);
  }
}
