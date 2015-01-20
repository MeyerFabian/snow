#|
  This file is a part of cl-voxelize project.
  Copyright (c) 2014 Masayuki Takagi (kamonama@gmail.com)
|#

(in-package :cl-user)
(defpackage cl-voxelize-asd
  (:use :cl :asdf))
(in-package :cl-voxelize-asd)

(defsystem cl-voxelize
  :version "0.1"
  :author "Masayuki Takagi"
  :license "LLGPL"
  :depends-on (:alexandria)
  :components ((:module "src"
                :components
                ((:file "cl-voxelize"))))
  :description ""
  :long-description
  #.(with-open-file (stream (merge-pathnames
                             #p"README.markdown"
                             (or *load-pathname* *compile-file-pathname*))
                            :if-does-not-exist nil
                            :direction :input)
      (when stream
        (let ((seq (make-array (file-length stream)
                               :element-type 'character
                               :fill-pointer t)))
          (setf (fill-pointer seq) (read-sequence seq stream))
          seq)))
  :in-order-to ((test-op (load-op cl-voxelize-test))))
