#|
  This file is a part of cl-voxelize project.
  Copyright (c) 2014 Masayuki Takagi (kamonama@gmail.com)
|#

(in-package :cl-user)
(defpackage cl-voxelize-test-asd
  (:use :cl :asdf))
(in-package :cl-voxelize-test-asd)

(defsystem cl-voxelize-test
  :author "Masayuki Takagi"
  :license "LLGPL"
  :depends-on (:cl-voxelize
               :cl-test-more)
  :components ((:module "t"
                :components
                ((:file "cl-voxelize"))))
  :perform (load-op :after (op c) (asdf:clear-system c)))
