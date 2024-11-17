
template<>
boost::asio::awaitable<void> UserHandler::HandleClassedEvent(std::shared_ptr<SuccessCheckLogPas> event)
{
	uuid_ = event->GetUuid();
	co_return;
}