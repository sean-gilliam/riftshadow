@extends('vendor.installer.layouts.master')

@section('template_title')
	{{ trans('installer_messages.environment.wizard.templateTitle') }}
@endsection

@section('title')
	<i class="fas fa-magic fa-fw" aria-hidden="true"></i>
	{!! trans('installer_messages.environment.wizard.title') !!}
@endsection

@section('container')
	<div class="tabs tabs-full">

		<input id="tab1" type="radio" name="tabs" class="tab-input" checked />
		<label for="tab1" class="tab-label">
			<i class="fas fa-cog fa-2x fa-fw" aria-hidden="true"></i>
			<br />
			{{ trans('installer_messages.environment.wizard.tabs.environment') }}
		</label>

		<input id="tab2" type="radio" name="tabs" class="tab-input" />
		<label for="tab2" class="tab-label">
			<i class="fas fa-database fa-2x fa-fw" aria-hidden="true"></i>
			<br />
			{{ trans('installer_messages.environment.wizard.tabs.database') }}
		</label>

		<input id="tab3" type="radio" name="tabs" class="tab-input" />
		<label for="tab3" class="tab-label">
			<i class="fas fa-cogs fa-2x fa-fw" aria-hidden="true"></i>
			<br />
			{{ trans('installer_messages.environment.wizard.tabs.application') }}
		</label>

		<form method="post" action="{{ route('LaravelInstaller::environmentSaveWizard') }}" class="tabs-wrap">
			<div class="tab" id="tab1content">
				<input type="hidden" name="_token" value="{{ csrf_token() }}">

				<div class="form-group {{ $errors->has('app_name') ? ' has-error ' : '' }}">
					<label for="app_name">
						{{ trans('installer_messages.environment.wizard.form.app_name_label') }}
					</label>
					<input type="text" name="app_name" id="app_name" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.app_name_placeholder') }}" />
					@if ($errors->has('app_name'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('app_name') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('environment') ? ' has-error ' : '' }}">
					<label for="environment">
						{{ trans('installer_messages.environment.wizard.form.app_environment_label') }}
					</label>
					<select name="environment" id="environment" onchange='checkEnvironment(this.value);'>
						<option value="local" selected>{{ trans('installer_messages.environment.wizard.form.app_environment_label_local') }}</option>
						<option value="development">{{ trans('installer_messages.environment.wizard.form.app_environment_label_developement') }}</option>
						<option value="qa">{{ trans('installer_messages.environment.wizard.form.app_environment_label_qa') }}</option>
						<option value="production">{{ trans('installer_messages.environment.wizard.form.app_environment_label_production') }}</option>
						<option value="other">{{ trans('installer_messages.environment.wizard.form.app_environment_label_other') }}</option>
					</select>
					<div id="environment_text_input" style="display: none;">
						<input type="text" name="environment_custom" id="environment_custom" placeholder="{{ trans('installer_messages.environment.wizard.form.app_environment_placeholder_other') }}"/>
					</div>
					@if ($errors->has('app_name'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('app_name') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('app_debug') ? ' has-error ' : '' }}">
					<label for="app_debug">
						{{ trans('installer_messages.environment.wizard.form.app_debug_label') }}
					</label>
					<label for="app_debug_true">
						<input type="radio" name="app_debug" id="app_debug_true" value=true checked />
						{{ trans('installer_messages.environment.wizard.form.app_debug_label_true') }}
					</label>
					<label for="app_debug_false">
						<input type="radio" name="app_debug" id="app_debug_false" value=false />
						{{ trans('installer_messages.environment.wizard.form.app_debug_label_false') }}
					</label>
					@if ($errors->has('app_debug'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('app_debug') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('app_url') ? ' has-error ' : '' }}">
					<label for="app_url">
						{{ trans('installer_messages.environment.wizard.form.app_url_label') }}
					</label>
					<input type="url" name="app_url" id="app_url" value="http://localhost" placeholder="{{ trans('installer_messages.environment.wizard.form.app_url_placeholder') }}" />
					@if ($errors->has('app_url'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('app_url') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('log_channel') ? ' has-error ' : '' }}">
					<label for="log_channel">
						{{ trans('installer_messages.environment.wizard.form.log_channel_label') }}
					</label>
					<input type="text" name="log_channel" id="log_channel" value="stack" placeholder="{{ trans('installer_messages.environment.wizard.form.log_channel_placeholder') }}" />
					@if ($errors->has('log_channel'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('log_channel') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('player_directory') ? ' has-error ' : '' }}">
					<label for="player_directory">
						{{ trans('installer_messages.environment.wizard.form.player_directory_label') }}
					</label>
					<input type="text" name="player_directory" id="player_directory" value="/../player/" placeholder="{{ trans('installer_messages.environment.wizard.form.player_directory_placeholder') }}" />
					@if ($errors->has('player_directory'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('player_directory') }}
						</span>
					@endif
				</div>

				<div class="buttons">
					<button class="button" onclick="showDatabaseSettings();return false">
						{{ trans('installer_messages.environment.wizard.form.buttons.setup_database') }}
						<i class="fas fa-angle-right fa-fw" aria-hidden="true"></i>
					</button>
				</div>
			</div>
			<div class="tab" id="tab2content">

				<div class="form-group {{ $errors->has('database_connection') ? ' has-error ' : '' }}">
					<label for="database_connection">
						{{ trans('installer_messages.environment.wizard.form.db_connection_label') }}
					</label>
					<select name="database_connection" id="database_connection">
						<option value="mysql" selected>{{ trans('installer_messages.environment.wizard.form.db_connection_label_mysql') }}</option>
						<option value="sqlite">{{ trans('installer_messages.environment.wizard.form.db_connection_label_sqlite') }}</option>
						<option value="pgsql">{{ trans('installer_messages.environment.wizard.form.db_connection_label_pgsql') }}</option>
						<option value="sqlsrv">{{ trans('installer_messages.environment.wizard.form.db_connection_label_sqlsrv') }}</option>
					</select>
					@if ($errors->has('database_connection'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_connection') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_hostname') ? ' has-error ' : '' }}">
					<label for="database_hostname">
						{{ trans('installer_messages.environment.wizard.form.db_host_label') }}
					</label>
					<input type="text" name="database_hostname" id="database_hostname" value="127.0.0.1" placeholder="{{ trans('installer_messages.environment.wizard.form.db_host_placeholder') }}" />
					@if ($errors->has('database_hostname'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_hostname') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_port') ? ' has-error ' : '' }}">
					<label for="database_port">
						{{ trans('installer_messages.environment.wizard.form.db_port_label') }}
					</label>
					<input type="number" name="database_port" id="database_port" value="3306" placeholder="{{ trans('installer_messages.environment.wizard.form.db_port_placeholder') }}" />
					@if ($errors->has('database_port'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_port') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_name') ? ' has-error ' : '' }}">
					<label for="database_name">
						{{ trans('installer_messages.environment.wizard.form.db_name_label') }}
					</label>
					<input type="text" name="database_name" id="database_name" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.db_name_placeholder') }}" />
					@if ($errors->has('database_name'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_name') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_username') ? ' has-error ' : '' }}">
					<label for="database_username">
						{{ trans('installer_messages.environment.wizard.form.db_username_label') }}
					</label>
					<input type="text" name="database_username" id="database_username" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.db_username_placeholder') }}" />
					@if ($errors->has('database_username'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_username') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_password') ? ' has-error ' : '' }}">
					<label for="database_password">
						{{ trans('installer_messages.environment.wizard.form.db_password_label') }}
					</label>
					<input type="password" name="database_password" id="database_password" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.db_password_placeholder') }}" />
					@if ($errors->has('database_password'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_password') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_hostname2') ? ' has-error ' : '' }}">
					<label for="database_hostname2">
						{{ trans('installer_messages.environment.wizard.form.db_host_label2') }}
					</label>
					<input type="text" name="database_hostname2" id="database_hostname2" value="127.0.0.1" placeholder="{{ trans('installer_messages.environment.wizard.form.db_host_placeholder') }}" />
					@if ($errors->has('database_hostname2'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_hostname2') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_port2') ? ' has-error ' : '' }}">
					<label for="database_port2">
						{{ trans('installer_messages.environment.wizard.form.db_port_label2') }}
					</label>
					<input type="number" name="database_port2" id="database_port2" value="3306" placeholder="{{ trans('installer_messages.environment.wizard.form.db_port_placeholder') }}" />
					@if ($errors->has('database_port2'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_port2') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_name2') ? ' has-error ' : '' }}">
					<label for="database_name2">
						{{ trans('installer_messages.environment.wizard.form.db_name_label2') }}
					</label>
					<input type="text" name="database_name2" id="database_name2" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.db_name_placeholder') }}" />
					@if ($errors->has('database_name2'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_name2') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_username2') ? ' has-error ' : '' }}">
					<label for="database_username2">
						{{ trans('installer_messages.environment.wizard.form.db_username_label2') }}
					</label>
					<input type="text" name="database_username2" id="database_username2" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.db_username_placeholder') }}" />
					@if ($errors->has('database_username2'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_username2') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('database_password2') ? ' has-error ' : '' }}">
					<label for="database_password2">
						{{ trans('installer_messages.environment.wizard.form.db_password_label2') }}
					</label>
					<input type="password" name="database_password2" id="database_password2" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.db_password_placeholder') }}" />
					@if ($errors->has('database_password2'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_password2') }}
						</span>
					@endif
				</div>

				<div class="buttons">
					<button class="button" onclick="showApplicationSettings();return false">
						{{ trans('installer_messages.environment.wizard.form.buttons.setup_application') }}
						<i class="fas fa-angle-right fa-fw" aria-hidden="true"></i>
					</button>
				</div>
			</div>
			<div class="tab" id="tab3content">
				<div class="block">
					<input type="radio" name="appSettingsTabs" id="appSettingsTab1" value="null" checked />
					<label for="appSettingsTab1">
						<span>
							{{ trans('installer_messages.environment.wizard.form.app_tabs.broadcasting_title') }}
						</span>
					</label>

					<div class="info">
						<div class="form-group {{ $errors->has('broadcast_driver') ? ' has-error ' : '' }}">
							<label for="broadcast_driver">{{ trans('installer_messages.environment.wizard.form.app_tabs.broadcasting_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/broadcasting" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="broadcast_driver" id="broadcast_driver" value="log" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.broadcasting_placeholder') }}" />
							@if ($errors->has('broadcast_driver'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('broadcast_driver') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('cache_driver') ? ' has-error ' : '' }}">
							<label for="cache_driver">{{ trans('installer_messages.environment.wizard.form.app_tabs.cache_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/cache" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="cache_driver" id="cache_driver" value="file" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.cache_placeholder') }}" />
							@if ($errors->has('cache_driver'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('cache_driver') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('session_driver') ? ' has-error ' : '' }}">
							<label for="session_driver">{{ trans('installer_messages.environment.wizard.form.app_tabs.session_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/session" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="session_driver" id="session_driver" value="file" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.session_placeholder') }}" />
							@if ($errors->has('session_driver'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('session_driver') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('queue_driver') ? ' has-error ' : '' }}">
							<label for="queue_driver">{{ trans('installer_messages.environment.wizard.form.app_tabs.queue_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/queues" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="queue_driver" id="queue_driver" value="sync" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.queue_placeholder') }}" />
							@if ($errors->has('queue_driver'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('queue_driver') }}
								</span>
							@endif
						</div>
					</div>
				</div>
				<div class="block">
					<input type="radio" name="appSettingsTabs" id="appSettingsTab2" value="null"/>
					<label for="appSettingsTab2">
						<span>
							{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_label') }}
						</span>
					</label>
					<div class="info">
						<div class="form-group {{ $errors->has('redis_hostname') ? ' has-error ' : '' }}">
							<label for="redis_hostname">
								{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_host') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/redis" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="redis_hostname" id="redis_hostname" value="127.0.0.1" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_host') }}" />
							@if ($errors->has('redis_hostname'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('redis_hostname') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('redis_password') ? ' has-error ' : '' }}">
							<label for="redis_password">{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_password') }}</label>
							<input type="password" name="redis_password" id="redis_password" value="null" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_password') }}" />
							@if ($errors->has('redis_password'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('redis_password') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('redis_port') ? ' has-error ' : '' }}">
							<label for="redis_port">{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_port') }}</label>
							<input type="number" name="redis_port" id="redis_port" value="6379" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_port') }}" />
							@if ($errors->has('redis_port'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('redis_port') }}
								</span>
							@endif
						</div>
					</div>
				</div>
				<div class="block">
					<input type="radio" name="appSettingsTabs" id="appSettingsTab3" value="null"/>
					<label for="appSettingsTab3">
						<span>
							{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_label') }}
						</span>
					</label>
					<div class="info">
						<div class="form-group {{ $errors->has('mail_driver') ? ' has-error ' : '' }}">
							<label for="mail_driver">
								{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_driver_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/mail" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="mail_driver" id="mail_driver" value="smtp" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_driver_placeholder') }}" />
							@if ($errors->has('mail_driver'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_driver') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_host') ? ' has-error ' : '' }}">
							<label for="mail_host">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_host_label') }}</label>
							<input type="text" name="mail_host" id="mail_host" value="smtp.mailtrap.io" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_host_placeholder') }}" />
							@if ($errors->has('mail_host'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_host') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_port') ? ' has-error ' : '' }}">
							<label for="mail_port">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_port_label') }}</label>
							<input type="number" name="mail_port" id="mail_port" value="2525" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_port_placeholder') }}" />
							@if ($errors->has('mail_port'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_port') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_username') ? ' has-error ' : '' }}">
							<label for="mail_username">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_username_label') }}</label>
							<input type="text" name="mail_username" id="mail_username" value="null" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_username_placeholder') }}" />
							@if ($errors->has('mail_username'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_username') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_password') ? ' has-error ' : '' }}">
							<label for="mail_password">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_password_label') }}</label>
							<input type="text" name="mail_password" id="mail_password" value="null" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_password_placeholder') }}" />
							@if ($errors->has('mail_password'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_password') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_encryption') ? ' has-error ' : '' }}">
							<label for="mail_encryption">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_encryption_label') }}</label>
							<input type="text" name="mail_encryption" id="mail_encryption" value="null" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_encryption_placeholder') }}" />
							@if ($errors->has('mail_encryption'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_encryption') }}
								</span>
							@endif
						</div>
					</div>
				</div>
				<div class="block margin-bottom-2">
					<input type="radio" name="appSettingsTabs" id="appSettingsTab4" value="null"/>
					<label for="appSettingsTab4">
						<span>
							{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_label') }}
						</span>
					</label>
					<div class="info">
						<div class="form-group {{ $errors->has('pusher_app_id') ? ' has-error ' : '' }}">
							<label for="pusher_app_id">
								{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_id_label') }}
								<sup>
									<a href="https://pusher.com/docs/server_api_guide" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="pusher_app_id" id="pusher_app_id" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_id_palceholder') }}" />
							@if ($errors->has('pusher_app_id'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('pusher_app_id') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('pusher_app_key') ? ' has-error ' : '' }}">
							<label for="pusher_app_key">{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_key_label') }}</label>
							<input type="text" name="pusher_app_key" id="pusher_app_key" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_key_palceholder') }}" />
							@if ($errors->has('pusher_app_key'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('pusher_app_key') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('pusher_app_secret') ? ' has-error ' : '' }}">
							<label for="pusher_app_secret">{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_secret_label') }}</label>
							<input type="password" name="pusher_app_secret" id="pusher_app_secret" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_secret_palceholder') }}" />
							@if ($errors->has('pusher_app_secret'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('pusher_app_secret') }}
								</span>
							@endif
						</div>
					</div>
				</div>
				<div class="buttons">
					<button class="button" type="submit">
						{{ trans('installer_messages.environment.wizard.form.buttons.install') }}
						<i class="fas fa-angle-right fa-fw" aria-hidden="true"></i>
					</button>
				</div>
			</div>
		</form>

	</div>
@endsection

@section('scripts')
	<script type="text/javascript">
		function checkEnvironment(val) {
			var element=document.getElementById('environment_text_input');
			if(val=='other') {
				element.style.display='block';
			} else {
				element.style.display='none';
			}
		}
		function showDatabaseSettings() {
			document.getElementById('tab2').checked = true;
		}
		function showApplicationSettings() {
			document.getElementById('tab3').checked = true;
		}
	</script>
@endsection
