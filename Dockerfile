FROM php:7.4-apache
RUN docker-php-ext-install -j$(nproc) pdo pdo_mysql
COPY --chown=www-data:www-data . /var/www/html
EXPOSE 80
