#|
  This file is a part of cl-voxelize project.
  Copyright (c) 2014 Masayuki Takagi (kamonama@gmail.com)
|#

(in-package :cl-user)
(defpackage cl-voxelize-examples-asd
  (:use :cl :asdf))
(in-package :cl-voxelize-examples-asd)

(defsystem cl-voxelize-examples
  :author "Masayuki Takagi"
  :license "LLGPL"
  :depends-on (:cl-voxelize
               :cl-ply)
  :components ((:module "examples"
                :components
                ((:file "bunny"))))
  :perform (load-op :after (op c) (asdf:clear-system c)))
