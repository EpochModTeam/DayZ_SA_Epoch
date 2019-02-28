pipeline {
  agent {
    node {
      label 'testing'
    }

  }
  stages {
    stage('Release') {
      steps {
        bat 'C:/GITTEMP/release_dzsa_epoch_client_test_public.cmd'
      }
    }
  }
}
