FROM tensorflow/tensorflow:latest-py3

RUN pip install https://github.com/ipython-contrib/jupyter_contrib_nbextensions/tarball/master \
                jupyter_contrib_nbextensions \
                influxdb

RUN jupyter contrib nbextension install --user && jupyter nbextensions_configurator enable --user
