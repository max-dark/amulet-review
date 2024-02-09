FROM php:8-apache
RUN curl -sS https://getcomposer.org/installer -o composer-setup.php && \
    php composer-setup.php --install-dir=/usr/local/bin --filename=composer && \
    rm composer-setup.php
RUN docker-php-ext-install -j$(nproc) pdo pdo_mysql
COPY --chown=www-data:www-data . /var/www/html
WORKDIR /var/www/html
USER www-data
RUN composer install
USER root
EXPOSE 80
