<?php

namespace Tests;

use StatonLab\TripalTestSuite\DBTransaction;
use StatonLab\TripalTestSuite\TripalTestCase;


class FormTest extends TripalTestCase {

  // Uncomment to auto start and rollback db transactions per test method.
  use DBTransaction;

  public function test_tripal_quick_fasta_fetch_features_for_sequence_test() {
    $features = tripal_quick_fasta_fetch_features_for_sequence();
    $this->assertNotEmpty($features);
  }

}
